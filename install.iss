[Setup]
AppName = SSHStart
AppVersion = 1.1.0
DefaultDirName = {autopf}\SSHStart
OutputBaseFilename = SSHStart Installer
PrivilegesRequiredOverridesAllowed = Dialog
ArchitecturesInstallIn64BitMode = x64
DefaultGroupName = SSHStart
SetupIconFile = img\icon.ico
WizardSmallImageFile = img\icon.bmp
UninstallDisplayName = SSHStart
UninstallDisplayIcon = {app}\bin\sshstart.exe,0
ChangesEnvironment = yes
OutputDir = x64
AppPublisher = Brandon Fowler
WizardImageFile = img\icon.bmp
WizardImageStretch = no

[Types]
Name: "full"; Description: "Full installation"
Name: "basic"; Description: "Installation without any system integrations"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: desktop; Description: Create a desktop icon; Types: full basic
Name: start; Description: Add to start menu; Types: full basic
Name: sshs; Description: Add sshs shortcut to bin; Types: full basic
Name: terminal; Description: Create a Windows Terminal profile; Types: full
Name: path; Description: Add to path; Types: full

[Files]
Source: "x64\Release\SSHStart.exe"; DestDir: "{app}\bin"; DestName: sshstart.exe
Source: "img\icon.ico"; DestDir: "{app}"

[Dirs]
Name: "{app}\bin"
Name: "{commonappdata}\Microsoft\Windows Terminal\Fragments\SSHStart"; Components: terminal; Check: IsAdminLoggedOn
Name: "{localappdata}\Microsoft\Windows Terminal\Fragments\SSHStart"; Components: terminal; Check: not IsAdminLoggedOn

[Icons]
Name: "{commondesktop}\SSHStart"; Filename: "{app}\bin\sshstart.exe"; Components: desktop; Check: IsAdminLoggedOn
Name: "{userdesktop}\SSHStart"; Filename: "{app}\bin\sshstart.exe"; Components: desktop; Check: not IsAdminLoggedOn
Name: "{group}\SSHStart"; Filename: "{app}\bin\sshstart.exe"; Components: start;

[Registry]
Root: HKCU; Subkey: "Environment"; \
	ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\bin"; Components: path; \
	Check: not IsAdminLoggedOn and PathNeeded(false, 'Environment')

Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
	ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\bin"; Components: path; \
	Check: IsAdminLoggedOn and PathNeeded(true, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment')

[Run]
Filename: "{app}\bin\sshstart.exe"; Description: "Launch SSHStart"; Flags: postinstall

[UninstallDelete]
// File must be deleted first, so Dirs entry does not delete folder
Type: files; Name: {commonappdata}\Microsoft\Windows Terminal\Fragments\SSHStart\SSHStart.json
Type: files; Name: {localappdata}\Microsoft\Windows Terminal\Fragments\SSHStart\SSHStart.json
Type: dirifempty; Name: {commonappdata}\Microsoft\Windows Terminal\Fragments\SSHStart
Type: dirifempty; Name: {localappdata}\Microsoft\Windows Terminal\Fragments\SSHStart

[Code]
function PathNeeded(UseSystem: Boolean; Key: String): Boolean;
	var
		Root: integer;
		Path: String;
	begin
	
    if UseSystem then begin
		Root := HKEY_LOCAL_MACHINE;
    end else begin
		Root := HKEY_CURRENT_USER;
    end;

	if not RegQueryStringValue(Root, Key, 'Path', Path) then begin
		Result := True;
		exit;
	end;
	
	Result := Pos(ExpandConstant(';{app}\bin;'), ';' + Path + ';') = 0;
end;

procedure RemovePath(UseSystem: Boolean; Key: String);
var
    Root: Integer;
	Path: String;
    Match: Integer;
begin
    if UseSystem then begin
		Root := HKEY_LOCAL_MACHINE;
    end else begin
		Root := HKEY_CURRENT_USER;
    end;

    if not RegQueryStringValue(Root, Key, 'Path', Path) then exit;

    Match := Pos(ExpandConstant(';{app}\bin;'), ';' + Path + ';');
    
	if Match = 0 then exit;

    Delete(Path, Match - 1, Length(ExpandConstant('{app}\bin')) + 1);
    RegWriteStringValue(Root, Key, 'Path', Path)
end;

procedure CurStepChanged(CurStep:TSetupStep);
	var JSONPath, AppPath, AppPathEsc: String;
	begin

	if CurStep<>ssPostInstall then Exit;
	
	AppPath := ExpandConstant('{app}');

	if IsComponentSelected('terminal') then begin
		if IsAdminInstallMode() then begin
			JSONPath := ExpandConstant('{commonappdata}\Microsoft\Windows Terminal\Fragments\SSHStart\SSHStart.json')
		end
		else begin
			JSONPath := ExpandConstant('{localappdata}\Microsoft\Windows Terminal\Fragments\SSHStart\SSHStart.json')
		end;		 

		AppPathEsc := AppPath;
		StringChangeEx(AppPathEsc, '\', '\\', True);

		SaveStringToFile(
			JSONPath,
			'{"profiles":[{"guid":"{e7d3c5fe-cd36-5b65-9dba-4d51e4c6b5d5}","name":"SSHStart","commandline":"' + AppPathEsc + '\\bin\\sshstart.exe","icon":"' + AppPathEsc + '\\icon.ico"}]}',
			False
		);
	end;

	if IsComponentSelected('sshs') then begin
		SaveStringToFile(AppPath + '\bin\sshs.cmd', + '@"' + AppPath + '\bin\sshstart.exe"', False);
	end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep<>usPostUninstall then Exit;

    RemovePath(false, 'Environment');
	RemovePath(true, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment');
end;
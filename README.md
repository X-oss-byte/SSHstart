# SSHStart - Secure Shell Start

SSHStart allows you to easily view, edit, and launch your saved SSH hosts. With it you can list and choose a SSH host to enter and view/edit your system-wide and user-specific SSH host.

## Usage

`sshstart [choice]`

`choice` is an optional argument representing a choice from the SSHStart menu to choose. The menu will still be presented if there is an error with this choice.

## Integrations

The installer offers integration into the Windows Terminal and the system path. The installer also defines the alias `sshs`, making it easier to launch SSHStart from a terminal.

## Building

SSHStart is built with Visual Studio, which offers the easiest way to build SSHStart. The installer is built from `install.iss` with Inno Setup. Tests can also be run in Visual Studio. They are located in the "tests" project.

## Screenshot

![Screenshot of SSHStart running in Windows Terminal](img/screenshot.png)

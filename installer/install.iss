#ifndef Target
  #define Target "release"
#endif
#ifndef FilesDir
  #define FilesDir "..\CHeroBot\bin\" + Target
#endif
#define MyAppName "CHeroBot"
#define MyAppPublisher "Nathan Martini"
#define MyAppExeName "CHeroBot.exe"
#define FileVerStr GetFileVersion(AddBackslash(FilesDir) + MyAppExeName)

[Setup]
AppId={{83A85841-6FDE-4D38-BF29-5AB1B134A459}
AppName={#MyAppName}
AppVersion={#FileVerStr}
AppVerName={#MyAppName} {#FileVerStr}
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf32}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
Compression=lzma
SolidCompression=yes
WizardImageFile=compiler:WIZMODERNIMAGE-IS.BMP
WizardSmallImageFile=logo.bmp
PrivilegesRequired=none
VersionInfoVersion={#FileVerStr}
VersionInfoTextVersion={#FileVerStr}
OutputBaseFilename=cherobot-{#FileVerStr}-{#Target}
OutputDir=..\release
CloseApplications=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[InstallDelete]
Type: filesandordirs; Name: "{app}\*";

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "{#FilesDir}\Sounds\*"; DestDir: "{app}\Sounds"; Flags: ignoreversion;
Source: "{#FilesDir}\*"; DestDir: "{app}"; Flags: ignoreversion; Excludes: "*.xml,*.pdb,debug.txt,*vshost*,*.iobj,*.obj,*.exp,*.ipdb,*.lib";

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Check: IsAdminLoggedOn;
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; Check: IsAdminLoggedOn;
<#
.DESCRIPTION
    Corrects an error in the CMake automatically generated VS project.
.PARAMETER builddir
    The path to the CMake build dir. You MUST use forward (\) slashes to specify the path.
.NOTES
    Author: Blair Urish
    Date:   June 28, 2016    
#>
param (
    [Parameter(Mandatory=$true,HelpMessage="Path to build directory. Use forward slashes (\)")][string]$builddir
)

$apporiginal = [Regex]::Escape((Get-Content $builddir/langdetect.vcxproj | Select-String "App.xaml.h"))
$mporiginal = [Regex]::Escape((Get-Content $builddir/langdetect.vcxproj | Select-String "MainPage.xaml.h"))

$appnew = '    <ClInclude Include="' + $PSScriptRoot + '\src\windows-uwp\App.xaml.h"><DependentUpon>' + $PSScriptRoot + '\src\windows-uwp\App.xaml</DependentUpon></ClInclude>'
$mpnew = '    <ClInclude Include="' + $PSScriptRoot + '\src\windows-uwp\MainPage.xaml.h"><DependentUpon>' + $PSScriptRoot + '\src\windows-uwp\MainPage.xaml</DependentUpon></ClInclude>'

(Get-Content $builddir/langdetect.vcxproj) | ForEach-Object {$_ -replace $apporiginal, $appnew} | Set-Content $builddir/langdetect.vcxproj
(Get-Content $builddir/langdetect.vcxproj) | ForEach-Object {$_ -replace $mporiginal, $mpnew} | Set-Content $builddir/langdetect.vcxproj
#requires -version 3

$BuildRoot = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

pushd code

& msbuild /nodeReuse:false "/p:Configuration=Release" EclipseLauncher.vcxproj

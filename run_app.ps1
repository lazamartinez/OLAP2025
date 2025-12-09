$env:QT_PLUGIN_PATH = "$PSScriptRoot\build"
$env:Path = "C:\Program Files\PostgreSQL\14\bin;C:\Qt\6.10.1\mingw_64\bin;$env:Path"
Write-Host "Iniciando SistemaOlap..."
& "$PSScriptRoot\build\SistemaOlap.exe"
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: El sistema falló con código de salida $LASTEXITCODE" -ForegroundColor Red
} else {
    Write-Host "Sistema finalizado correctamente." -ForegroundColor Green
}

@echo off
setlocal EnableDelayedExpansion

rem — clean & recreate built folder
if exist built (
  rmdir /s /q built
)
mkdir built

rem — each line: tag;output_name;cmake-flags
for %%L in (
  "shared;hft_shared_ptr;-DDEFAULT_PTRMODE=shared -DENABLE_POOL=ON -DDEFAULT_LAYOUT=multimap"
  "raw;hft_raw_ptr;-DDEFAULT_PTRMODE=raw    -DENABLE_POOL=OFF -DDEFAULT_LAYOUT=multimap"
  "align64_on;hft_align64_on;-DENABLE_ALIGN64=ON  -DDEFAULT_PTRMODE=shared -DENABLE_POOL=ON -DDEFAULT_LAYOUT=multimap"
  "align64_off;hft_align64_off;-DENABLE_ALIGN64=OFF -DDEFAULT_PTRMODE=shared -DENABLE_POOL=ON -DDEFAULT_LAYOUT=multimap"
  "pool_on;hft_pool_on;-DDEFAULT_PTRMODE=pool  -DENABLE_POOL=ON  -DDEFAULT_LAYOUT=multimap"
  "pool_off;hft_pool_off;-DDEFAULT_PTRMODE=shared -DENABLE_POOL=OFF -DDEFAULT_LAYOUT=multimap"
  "layout_multimap;hft_layout_multimap;-DDEFAULT_LAYOUT=multimap -DDEFAULT_PTRMODE=shared -DENABLE_POOL=ON"
  "layout_flat;hft_layout_flat;-DDEFAULT_LAYOUT=flat      -DDEFAULT_PTRMODE=shared -DENABLE_POOL=ON"
) do (
  for /f "tokens=1,2,3 delims=;" %%A in ("%%~L") do (
    set "TAG=%%A"
    set "EXE=%%B"
    set "CFG=%%C"

    echo.
    echo ================================
    echo Building !TAG! as !EXE!.exe with flags: !CFG!
    echo ================================

    rem re-configure
    cmake .. -G "NMake Makefiles" !CFG!
    if ERRORLEVEL 1 exit /b 1

    rem build
    nmake
    if ERRORLEVEL 1 exit /b 1

    rem copy the single hft_app.exe into a scenario-specific name
    copy /Y hft_app.exe built\!EXE!.exe >nul
  )
)

echo.
echo All builds complete. Executables are in build\built\
endlocal

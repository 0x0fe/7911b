@echo off

@echo *********************************************************************
@echo 			    AC791N SDK SD/U�������ļ�����
@echo *********************************************************************
@echo %date%

::��ִ��download.bat�����ļ�
call .\download.bat

if exist upgrade\*.ufw del upgrade\*.ufw

::�ٸ����ļ�
copy jl_isd.ufw upgrade\update.ufw

echo.
echo.
echo �����ļ����ƣ�update.ufw����update.ufw������SD��/U�̵ĸ�Ŀ¼���忨�ϵ缴��������2���Ӻ��Զ��رմ��ڣ�
echo.
choice /t 2 /d y /n >nul 

::pause
exit

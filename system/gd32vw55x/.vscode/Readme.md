
本文档提供支持在VSCode中编译、下载以及调试GD32VW553。

# 必需软件包

- GD32VW553 Release SDK
- 工具链以及Openocd
- 支持编译、下载以及调试功能的json文件

工具链和Openocd以及脚本文件和Release SDK按照下述文件结构存放。

# 文件结构

```
📦gd32vw553-release-sdk/
├── 📁.vscode/                                  # 编译、下载、调试脚本                  
│   ├── download.bat
│   ├── download.sh
│   ├── launch.json
│   ├── settings.json
│   ├── tasks.json
│   └── Readme.md                               # 本说明文件
├── 📁tools/
│   ├── gd32vw55x_toolchain_windows.7z.001      # windows 工具链 & Openocd
│   ├── gd32vw55x_toolchain_windows.7z.002
│   ├── gd32vw55x_toolchain_windows.7z.003
│   ├── xpack-openocd-0.11.0-3_windows.7z
│   ├── gd32vw55x_toolchain_linux.tar.gz00      # linux 工具链 & Openocd
│   ├── gd32vw55x_toolchain_linux.tar.gz01
│   ├── gd32vw55x_toolchain_linux.tar.gz02
│   ├── gd32vw55x_toolchain_linux.tar.gz03
│   ├── gd32vw55x_toolchain_linux.tar.gz04
│   ├── gd32vw55x_toolchain_linux.tar.gz05
│   ├── gd32vw55x_toolchain_linux.tar.gz06
│   └── xpack-openocd-0.11.0-3_linux.tar.gz
├── MBL/                                         # Release SDK 文件
├── MSDK/
├── ...
```


使用 VSCode 打开GD32VW553 Release SDK。

# 编译 MSDK

终端 -> 运行任务 -> 选择 [ Build MSDK]

即可完成对MSDK的编译，注意首次编译时，会自动解压工具链文件。

# 编译 MBL

终端 -> 运行任务 -> 选择 [ Build MBL]

# 编译 ALL

如需要编译 MSDK 和 MBL，选择 Build ALL 即可。

终端 -> 运行任务 -> 选择 [ Build ALL]

# 下载 MSDK

- 选择GDLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download MSDK (GDLink)]
- 选择JLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download MSDK (JLink)]

# 下载 MBL

- 选择GDLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download MBL (GDLink)]
- 选择JLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download MBL (JLink)]

# 下载 ALL

- 选择GDLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download ALL (GDLink)]
- 选择JLink方式进行下载：
	终端 -> 运行任务 -> 选择 [ Download ALL (JLink)]

# 调试

点击F5进行调试，同时加载了MBL和MSDK的符号表，另外支持GDLink 和 Jlink两种方式，以及调试时还可以选择是否需要重新Download。

- GDB调试 (GDLink, with Download)
- GDB调试 (GDLink)
- GDB调试 (JLink, with Download)
- GDB调试 (JLink)

# 下载指定bin文件

参考download.bat/download.sh中下载文件方法。

根据下载方式不同选在不同的Config File，以及指定待烧录的bin文件路径和烧录至Flash Address。

命令格式参考：
"tools\xpack-openocd-0.11.0-3_windows\bin\openocd.exe" -f "!CONFIG_FILE!" -c "init" -c "program !BIN_FILE! %FLASH_ADDR% verify reset" -c "exit"

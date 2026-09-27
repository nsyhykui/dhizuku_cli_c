# dhizuku-cli-c

Dhizuku Device Owner command-line client over TCP with TOTP authentication.

A C client that talks to the dhizuku-cli Android server app,
executes Device Owner commands via Dhizuku.

> Server app / 服务端 App:
> https://github.com/nsyhykui/dhizuku-cli

---

## English

### About

This is the C client for dhizuku-cli.

The Android server app is in the main repository:
https://github.com/nsyhykui/dhizuku-cli

It communicates with the Android server app over TCP, using TOTP for
authentication and AES-GCM for encryption. The server app executes
Device Owner commands via Dhizuku.

### Requirements

- C compiler (gcc or clang)
- OpenSSL (libssl + libcrypto)
- The Android server app running on your device

### Build

    make

The output binary is dcli in the project root.

### Install

    sudo make install

Default install path is /usr/local/bin. Override with PREFIX:

    make PREFIX=$HOME/.local install

In Termux:

    make PREFIX=$PREFIX install

### Quick Start

1. Install the Android server app from Releases.

2. Open the app, tap Check Dhizuku, then Start TCP Service.

3. Copy the key shown in the app and save it:

    echo "<key from the app>" > ~/.dcli_key

4. Run a command:

    dcli ping
    dcli lock_now
    dcli hide com.example.app

### Commands

| Command | Argument | Description |
|---------|----------|-------------|
| ping | — | Test connection |
| lock_now | — | Lock the screen |
| hide | package | Hide an app |
| unhide | package | Unhide an app |
| suspend | package | Suspend an app |
| resume | package | Resume an app |
| block_uninstall | package | Block uninstall |
| unblock_uninstall | package | Allow uninstall |
| status | <subcommand> | Query device status |

### Status Commands

| Subcommand | Description |
|------------|-------------|
| status hid | List hidden apps |
| status suspend | List suspended apps |
| status block_uninstall | List apps with uninstall blocked |
| status permission update | Rescan all apps and update cache |
| status permission <perm> | List apps with this permission |
| status permission --package <pkg> | List all permissions of an app |
| status permission <perm> --package <pkg> | Query one app's permission state |

### Options

    --host, -H <ip>    Server IP (default 127.0.0.1)
    --version, -V      Show version
    --help, -h         Show help
    --                 Stop option parsing

### LAN Mode

To control from another device on the same network:

1. In the app, change bind address to LAN (0.0.0.0)
2. On the client:

    dcli --host 192.168.1.100 ping

   Or set once:

    echo "192.168.1.100" > ~/.dcli_host

### Configuration

| File | Content |
|------|---------|
| ~/.dcli_key | TOTP shared key |
| ~/.dcli_host | Server IP (optional) |
| DCLI_KEY | Env var for key |
| DCLI_HOST | Env var for host |

Priority: command-line > env var > current dir file > home dir file > default.

### Security

- The TOTP key is the only credential. Keep it safe.
- All messages are encrypted with AES-GCM.
- Do not enable LAN mode on untrusted networks.

### Changelog

#### v1.1.0

- Added `status` commands
- Added `--version` / `-V`
- Added `--help` / `-h` support
- Colored output for errors and warnings
- Fixed `--package` being treated as unknown option
- Response reading now waits for EOF (supports multi-line output)

#### v1.0.0

- First release

---

## 简体中文

### 关于

这是 dhizuku-cli 的 C 客户端。

服务端 Android App 在主仓库：
https://github.com/nsyhykui/dhizuku-cli

它通过 TCP 与 Android 服务端通信，用 TOTP 做认证，用 AES-GCM 加密。
服务端通过 Dhizuku 执行 Device Owner 命令。

### 依赖

- C 编译器（gcc 或 clang）
- OpenSSL（libssl + libcrypto）
- 设备上运行的服务端 App

### 编译

    make

生成的二进制文件是项目根目录下的 dcli。

### 安装

    sudo make install

默认安装到 /usr/local/bin。用 PREFIX 改路径：

    make PREFIX=$HOME/.local install

Termux 里：

    make PREFIX=$PREFIX install

### 快速开始

1. 从 Releases 下载并安装 Android 服务端。

2. 打开 App，点检测 Dhizuku，再点启动 TCP 服务。

3. 复制 App 里显示的密钥，写入文件：

    echo "<App 里的密钥>" > ~/.dcli_key

4. 执行命令：

    dcli ping
    dcli lock_now
    dcli hide com.example.app

### 命令列表

| 命令 | 参数 | 说明 |
|------|------|------|
| ping | 无 | 测试连接 |
| lock_now | 无 | 立即锁屏 |
| hide | 包名 | 隐藏应用 |
| unhide | 包名 | 取消隐藏 |
| suspend | 包名 | 挂起应用 |
| resume | 包名 | 恢复挂起 |
| block_uninstall | 包名 | 阻止卸载 |
| unblock_uninstall | 包名 | 允许卸载 |
| status | <子命令> | 查询设备状态 |

### Status 命令

| 子命令 | 说明 |
|--------|------|
| status hid | 列出被隐藏的应用 |
| status suspend | 列出被挂起的应用 |
| status block_uninstall | 列出阻止卸载的应用 |
| status permission update | 重新扫描所有应用并更新缓存 |
| status permission <权限> | 列出拥有该权限的应用 |
| status permission --package <包名> | 列出该应用的所有权限 |
| status permission <权限> --package <包名> | 查询某应用某权限状态 |

### 选项

    --host, -H <ip>    服务端 IP（默认 127.0.0.1）
    --version, -V      显示版本
    --help, -h         显示帮助
    --                 停止解析后续选项

### 局域网模式

要从同网络的其他设备控制：

1. 在 App 里把监听地址改成局域网 (0.0.0.0)
2. 客户端执行：

    dcli --host 192.168.1.100 ping

   或者一次写好：

    echo "192.168.1.100" > ~/.dcli_host

### 配置

| 文件 | 内容 |
|------|------|
| ~/.dcli_key | TOTP 共享密钥 |
| ~/.dcli_host | 服务端 IP（可选） |
| DCLI_KEY | 密钥环境变量 |
| DCLI_HOST | 地址环境变量 |

优先级：命令行 > 环境变量 > 当前目录文件 > HOME 文件 > 默认值。

### 安全说明

- TOTP 密钥是唯一的认证凭据，请妥善保管。
- 所有消息都经过 AES-GCM 加密。
- 不要在不可信网络上开启局域网模式。

### 更新日志

#### v1.1.0

- 新增 status 命令
- 新增 --version / -V
- 新增 --help / -h 支持
- 错误与警告输出带颜色
- 修复 --package 被当成未知选项的问题
- 响应读取改为读到 EOF（支持多行输出）

#### v1.0.0

- 首个版本

---

## License

GPL-3.0. See LICENSE for details.

Copyright (C) 2026 nsyhykui

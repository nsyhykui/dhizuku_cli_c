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
    dcli list hidden
    dcli pm list packages -3

### Commands

Operation commands:

| Command | Description |
|---------|-------------|
| ping | Test connection |
| lock_now | Lock the screen |
| hide / unhide | Hide / unhide an app |
| suspend / resume | Suspend / resume an app |
| block_uninstall / unblock_uninstall | Block / allow uninstall |

Query commands:

| Command | Description |
|---------|-------------|
| list hidden | List hidden apps |
| list suspended | List suspended apps |
| list blocked | List apps with uninstall blocked |
| pm list packages [options] | List packages (same as pm list packages) |
| pm list permissions <perm> | List apps with this permission |
| pm list permissions --package <pkg> | List all permissions of an app |
| pm list permissions <perm> --package <pkg> | Query one app's permission state |
| cache update | Rescan all apps and update cache |
| status | Show server running status |

Permission management commands:

| Command | Description |
|---------|-------------|
| pm grant <pkg> <perm> | Grant a runtime permission |
| pm revoke <pkg> <perm> | Revoke a runtime permission |
| pm reset <pkg> <perm> | Reset a permission to default |

pm list packages supports the same options as Android's pm list packages:
-f -d -e -s -3 -i -u -U --uid, plus a package name filter. The only
exception is --user, which is not supported.

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

#### v2.0.0

- Breaking change: command structure and output protocol changed
- Added pm-style commands (pm list packages / pm list permissions / pm grant / pm revoke / pm reset)
- Added list hidden / list suspended / list blocked
- Added cache update
- Added status (client-side ping)
- Data commands no longer prefix output with Success
- Removed: status hid / status suspend / status block_uninstall / status permission xxx

#### v1.1.0

- Added status commands (superseded by v2.0.0)
- Added --version / -V
- Added --help / -h support
- Colored output for errors and warnings
- Fixed --package being treated as unknown option
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
    dcli list hidden
    dcli pm list packages -3

### 命令列表

操作类命令：

| 命令 | 说明 |
|------|------|
| ping | 测试连接 |
| lock_now | 立即锁屏 |
| hide / unhide | 隐藏 / 取消隐藏应用 |
| suspend / resume | 挂起 / 恢复应用 |
| block_uninstall / unblock_uninstall | 阻止 / 允许卸载 |

查询类命令：

| 命令 | 说明 |
|------|------|
| list hidden | 列出被隐藏的应用 |
| list suspended | 列出被挂起的应用 |
| list blocked | 列出阻止卸载的应用 |
| pm list packages [参数] | 列出应用（同 pm list packages） |
| pm list permissions <权限> | 列出拥有该权限的应用 |
| pm list permissions --package <包名> | 列出该应用的所有权限 |
| pm list permissions <权限> --package <包名> | 查询某应用某权限状态 |
| cache update | 重新扫描所有应用并更新缓存 |
| status | 显示服务端运行状态 |

权限管理命令：

| 命令 | 说明 |
|------|------|
| pm grant <包名> <权限> | 授予运行时权限 |
| pm revoke <包名> <权限> | 拒绝运行时权限 |
| pm reset <包名> <权限> | 恢复权限到默认状态 |

pm list packages 的参数和 Android 自带的 pm list packages 一致：
-f -d -e -s -3 -i -u -U --uid，另加包名过滤。唯一不支持的是 --user。

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

#### v2.0.0

- 破坏性更新：命令结构和输出协议都变了
- 新增 pm 风格命令（pm list packages / pm list permissions / pm grant / pm revoke / pm reset）
- 新增 list hidden / list suspended / list blocked
- 新增 cache update
- 新增 status（客户端本地 ping）
- 有数据的命令不再带 Success 前缀
- 删除：status hid / status suspend / status block_uninstall / status permission xxx

#### v1.1.0

- 新增 status 命令（v2.0.0 中被替代）
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

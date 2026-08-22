import os
import glob
Import("env")

PROJECT_DIR = env.subst("$PROJECT_DIR")

platform = env.get("PIOPLATFORM")
arch = env.get("BOARD_MCU", "")

# 读取客户项目的 build_flags，判断需要链接哪些联网 addon
build_flags = env.get("BUILD_FLAGS", [])
if isinstance(build_flags, str):
    build_flags_str = build_flags
else:
    build_flags_str = " ".join(str(f) for f in build_flags)
build_flags_lower = build_flags_str.lower()

# v1.3 起：debug 恒带（运行期 onDebug 门控），不再有 _debug 变体库；
# 联网功能拆为 addon 库（base 必链 + addon 按开关追加链接）
has_debug = "-dtbd_debug" in build_flags_lower or "-d tbd_debug" in build_flags_lower
has_ether = "-dtbd_ether" in build_flags_lower or "-d tbd_ether" in build_flags_lower
has_gsm   = "-dtbd_gsm"   in build_flags_lower or "-d tbd_gsm"   in build_flags_lower
has_gateway = "-dtbd_gateway" in build_flags_lower or "-d tbd_gateway" in build_flags_lower

if has_debug:
    print("[thingboot_device] NOTE: -DTBD_DEBUG 已废弃，日志恒带（device.onDebug 注册即输出），将链接 base 库")

base = None
if platform == "espressif8266":
    base = "esp8266"
elif platform == "espressif32":
    arch_l = arch.lower()
    if "s3" in arch_l:
        base = "esp32s3"
    elif "s2" in arch_l:
        base = "esp32s2"
    elif "c6" in arch_l:
        base = "esp32c6"
    elif "c3" in arch_l:
        base = "esp32c3"
    elif "c2" in arch_l or "c61" in arch_l:
        print(f"[thingboot_device] WARNING: ESP32-C2/C61 需要 ESP-IDF 组件方式，不在 Arduino SDK 支持范围内")
    else:
        base = "esp32"

def find_sdk_lib_dir(project_dir):
    """定位 SDK 的 lib/ 目录。

    优先通过 extra_script.py 自身所在目录（SDK 根目录）查找 lib/ 或 dist/lib/；
    若失败，再回退到项目目录下搜索常见的 PlatformIO 安装路径。
    """
    sdk_root = str(Dir('.').srcnode().abspath)
    candidates = [
        os.path.join(sdk_root, "lib"),
        os.path.join(sdk_root, "dist", "lib"),
    ]
    for path in candidates:
        if os.path.isdir(path):
            return path

    # Fallback：在 project_dir 下搜索（兼容旧布局/手动放置）
    patterns = [
        os.path.join(project_dir, ".pio", "libdeps", "*", "thingboot-device-esp-arduino-sdk", "lib"),
        os.path.join(project_dir, ".pio", "libdeps", "*", "thingboot-device-esp-arduino-sdk", "dist", "lib"),
        os.path.join(project_dir, ".pio", "libdeps", "*", "ThingBootSDK", "lib"),
        os.path.join(project_dir, ".pio", "libdeps", "*", "ThingBootSDK", "dist", "lib"),
        os.path.join(project_dir, ".pio", "libdeps", "*", "thingboot-device-esp-arduino-sdk-main", "lib"),
        os.path.join(project_dir, ".pio", "libdeps", "*", "thingboot-device-esp-arduino-sdk-main", "dist", "lib"),
        os.path.join(project_dir, "lib", "thingboot-device-esp-arduino-sdk", "lib"),
        os.path.join(project_dir, "lib", "thingboot-device-esp-arduino-sdk", "dist", "lib"),
        os.path.join(project_dir, "lib", "ThingBootSDK", "lib"),
        os.path.join(project_dir, "lib", "ThingBootSDK", "dist", "lib"),
        os.path.join(project_dir, "lib", "thingboot-device-esp-arduino-sdk-main", "lib"),
        os.path.join(project_dir, "lib", "thingboot-device-esp-arduino-sdk-main", "dist", "lib"),
    ]
    for pattern in patterns:
        for path in glob.glob(pattern):
            if os.path.isdir(path):
                return path
    return None


if base:
    lib_dir = find_sdk_lib_dir(PROJECT_DIR)
    if lib_dir:
        base_lib = f"libthingboot_device_{base}.a"
        addon_libs = []
        if has_ether:
            addon_libs.append(f"libthingboot_addon_net_ether_{base}.a")
        if has_gsm:
            addon_libs.append(f"libthingboot_addon_net_gsm_{base}.a")
        if has_gateway:
            addon_libs.append(f"libthingboot_addon_gateway_{base}.a")

        base_path = os.path.join(lib_dir, base_lib)
        if os.path.exists(base_path):
            env.Append(LIBPATH=[lib_dir])
            env.Prepend(LIBS=[base_lib[3:-2]])
            print("[thingboot_device] Linked: " + base_lib)
        else:
            print("[thingboot_device] WARNING: Not found " + base_path)

        # addon 后于 base 加入（LIBS 前置），最终顺序为 addon 在前、base 在后：
        # 用户经 install() 引用 addon 符号，addon 引用 base 的驱动表符号，
        # 反序会导致 ld 单趟扫描漏拉 base 对象
        for addon_lib in addon_libs:
            addon_path = os.path.join(lib_dir, addon_lib)
            if os.path.exists(addon_path):
                env.Append(LIBPATH=[lib_dir])
                env.Prepend(LIBS=[addon_lib[3:-2]])
                print("[thingboot_device] Linked addon: " + addon_lib)
            else:
                print(f"[thingboot_device] WARNING: 未找到联网 addon 库 {addon_path}")
                print(f"[thingboot_device] WARNING: 对应功能将不可用（install 返回 ERR_NETWORK_ABI_MISMATCH）")
    else:
        print("[thingboot_device] WARNING: 找不到 SDK lib 目录")

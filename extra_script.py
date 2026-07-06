import os
Import("env")

PROJECT_DIR = env.subst("$PROJECT_DIR")

platform = env.get("PIOPLATFORM")
arch = env.get("BOARD_MCU", "")

# 读取客户项目的 build_flags，判断需要哪种功能变体
build_flags = env.get("BUILD_FLAGS", [])
if isinstance(build_flags, str):
    build_flags_str = build_flags
else:
    build_flags_str = " ".join(str(f) for f in build_flags)
build_flags_lower = build_flags_str.lower()

# 新开关命名：TBD_DEBUG / TBD_ETHER / TBD_GSM
has_debug = "-dtbd_debug" in build_flags_lower or "-d tbd_debug" in build_flags_lower
has_ether = "-dtbd_ether" in build_flags_lower or "-d tbd_ether" in build_flags_lower
has_gsm   = "-dtbd_gsm"   in build_flags_lower or "-d tbd_gsm"   in build_flags_lower

def make_lib_name(base, has_ether, has_gsm, has_debug):
    parts = [base]
    if has_ether:
        parts.append("ether")
    if has_gsm:
        parts.append("gsm")
    if has_debug:
        parts.append("debug")
    return f"libthingboot_device_{'_'.join(parts)}.a"

def fallback_names(lib_name):
    """按优先级生成降级候选库名"""
    candidates = []
    # 1) 去掉 debug
    if "_debug.a" in lib_name:
        candidates.append(lib_name.replace("_debug.a", ".a"))
    # 2) 去掉 gsm
    if "_gsm" in lib_name:
        candidates.append(lib_name.replace("_gsm", ""))
    # 3) 去掉 ether
    if "_ether" in lib_name:
        candidates.append(lib_name.replace("_ether", ""))
    # 4) 仅保留 base（去掉所有特性）
    base_prefix = f"libthingboot_device_{base}"
    candidates.append(base_prefix + ".a")
    return candidates

lib_name = None

if platform == "espressif8266":
    base = "esp8266"
    lib_name = make_lib_name(base, has_ether, has_gsm, has_debug)
elif platform == "espressif32":
    arch_l = arch.lower()
    if "s3" in arch_l:
        base = "esp32s3"
    elif "c6" in arch_l:
        base = "esp32c6"
    else:
        base = "esp32"
    lib_name = make_lib_name(base, has_ether, has_gsm, has_debug)

if lib_name:
    # 优先使用 extra_script.py 自身所在目录定位 lib/。
    # 这样无论库是通过 lib_deps 安装到 .pio/libdeps/<env>/ 下，
    # 还是手动放到 lib/ 下，都能找到 .a 文件。
    sdk_dir = os.path.dirname(os.path.abspath(__file__))
    candidates = [
        os.path.join(sdk_dir, "lib"),
        # 兼容旧的手动放置路径
        os.path.join(PROJECT_DIR, "lib", "thingboot-device-sdk", "lib"),
        os.path.join(PROJECT_DIR, "lib", "ThingBootSDK", "lib"),
        os.path.join(PROJECT_DIR, "lib", "thingboot-device-sdk-main", "lib"),
    ]
    lib_dir = None
    for c in candidates:
        if os.path.exists(c):
            lib_dir = c
            break
    if not lib_dir:
        lib_dir = candidates[0]

    lib_path = os.path.join(lib_dir, lib_name)

    # 如果首选变体不存在，按优先级尝试降级
    if not os.path.exists(lib_path):
        try:
            available = [f for f in os.listdir(lib_dir)
                         if f.startswith("libthingboot_device_") and f.endswith(".a")]
            chosen = None
            for fb in fallback_names(lib_name):
                if fb in available:
                    chosen = fb
                    break
            if not chosen:
                # 最后尝试同平台任意变体
                base_prefix = f"libthingboot_device_{base}"
                for f in available:
                    if f.startswith(base_prefix):
                        chosen = f
                        break
            if chosen:
                lib_name = chosen
                lib_path = os.path.join(lib_dir, lib_name)
        except Exception:
            pass

    if os.path.exists(lib_path):
        env.Append(LIBPATH=[lib_dir])
        env.Prepend(LIBS=[lib_name[3:-2]])
        print("[thingboot_device] Linked: " + lib_name)
    else:
        print("[thingboot_device] WARNING: Not found " + lib_path)

# Docker Development Environment

This directory contains Docker support for building the GD32VW55x WiFi/BLE SDK.
The container image includes the necessary cross‑toolchain and dependencies, avoiding the need to configure your host machine directly.

---

## 1. Build the Image

Run the following command in this directory to create an image named `gd32vw553-dev:v1`:

```bash
docker build -t gd32vw553-dev:v1 .
```

> **Tip**
> - You can modify the base image or toolchain version in the `Dockerfile` if needed.
> - Add `--progress=plain` during build to see verbose output.

---

## 2. Run the Container

When launching the container, you must mount the SDK source directory (default path inside the container is `/project`) and start an interactive shell:

For example, SDK source directory is ~/linux_work/github/GD32VW55x_WiFi_BLE_SDK.
```bash
sudo docker run --rm --privileged -it -v /dev/bus/usb:/dev/bus/usb -v ~/linux_work/github/GD32VW55x_WiFi_BLE_SDK:/project gd32vw553-dev:v1
```

## Parameters explained:

- `--rm`: Remove container after exit.
- `-it`: Interactive terminal.
- `-v <host path>:/project`: Mount local SDK folder into the container.
- `gd32vw553-dev:v1`: Image to run.

Adjust the mount path or add options (network, device access, etc.) as needed.

---

## 3. Build the SDK Inside the Container

Once inside, change to the project directory and execute the build script:

```bash
./cmake_build.sh app -j
```

Supported build targets include:

- `app` (build all applications, e.g. MSDK/MBL)
- `msdk`, `mbl`, etc. for individual components
- `clean` to remove build artifacts

The `-j` flag enables parallel builds; specify a number (e.g. `-j8`) to match CPU cores.

---

## 4. Common Examples

```bash
# Build everything
./cmake_build.sh app

# Build only MSDK
./cmake_build.sh app msdk

# Clean build products
./cmake_build.sh app clean
```


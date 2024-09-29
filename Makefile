.PHONY:buildroot-download buildroot-config buildroot kernel-download toolchain-download kernel-config kernel Image Rootfs
all: buildroot kernel
buildroot-download:
	if [ ! -e buildroot ];then \
		wget https://buildroot.org/downloads/buildroot-2024.02.5.tar.xz; \
		tar xvf buildroot-2024.02.5.tar.xz; rm buildroot-2024.02.5.tar.xz; \
		mv buildroot-2024.02.5 buildroot; \
	fi
# make qemu_aarch64_virt_defconfig, make menuconfig를 통해 미리 설정해둔 .config파일을 복사해 사용
buildroot-config: buildroot-download
	cp buildroot_config buildroot/.config
	cd buildroot; make olddefconfig
buildroot: buildroot-config
	cd buildroot; make -j$(shell nproc --all)
buildroot/output/images/rootfs.ext4:
	if [ ! -e buildroot/output/images/rootfs.ext4 ];then \
		make buildroot; \
	fi
kernel-download:
	if [ ! -e linux ];then \
		wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.47.tar.xz; \
		tar xvf linux-6.6.47.tar.xz; rm linux-6.6.47.tar.xz; \
		mv linux-6.6.47 linux; \
	fi
toolchain-download:
	if [ ! -e gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu ];then \
		wget https://developer.arm.com/-/media/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz?rev=1cb9c51b94f54940bdcccd791451cec3&hash=A56CA491FA630C98F7162BC1A302F869; \
		tar xvf gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz; \
		rm gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz; \
	fi
# buildroot에서 defconfig파일을 복사해 ARCH=64 make qemu_defconfig한 .config파일을 복사해 사용 
kernel-config: kernel-download toolchain-download
	cp kernel_config linux/.config
	cd linux; ARCH=arm64 make olddefconfig
# sudo apt install libelf-dev libssl-dev bison flex가 필요
kernel: kernel-config
	cd linux; \
	ARCH=arm64 CROSS_COMPILE=$(shell pwd)/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu- make -j$(shell nproc --all)
linux/arch/arm64/boot/Image:
	if [ ! -e linux/arch/arm64/boot/Image ];then \
		make kernel; \
	fi
run: buildroot/output/images/rootfs.ext4 linux/arch/arm64/boot/Image
	qemu-system-aarch64 \
	-kernel linux/arch/arm64/boot/Image \
	-drive format=raw,file=buildroot/output/images/rootfs.ext4,if=virtio \
	-append "root=/dev/vda console=ttyAMA0 nokaslr" \
	-nographic \
	-M virt \
	-cpu cortex-a72 \
	-m 2G \
	-smp 2

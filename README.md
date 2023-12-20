# ElfosV5
All things Elf/OS V5

With the demise of elf-emulation.com I have created this GitHub repository
to house all materials for Elf/OS v5.  Keep an eye on this repository for
any news or updates concerning Elf/OS v5.

Status as of 12/20/2023:

  All files currently posted are considered preliminary.  These files
have not been 100% tested, as such be sure to have a backup of your 
filesystem before using any of these files.

Files:

  basetools5.lbr  - Base /bin tools for Elf/OS v5

  eosinst1.intel  - 1802-based ROM-based installer for Elf/OS v5 for type I
                    filesystem (LAT-16)

  eosinst2.intel  - 1802-based ROM-based installer for Elf/OS v5 for type II
                    filesystem (LAT-32)

  eosinst1r.intel - 1802-based RAM-based installer for Elf/OS v5 for type I
                    filesystem (LAT-16)

  eosinst2r.intel - 1802-based RAM-based installer for Elf/OS v5 for type II
                    filesystem (LAT-32)

  FsGen1/         - PC-based filesystem image generator for type I filesystem
                    (LAT-16)

  FsGen2/         - PC-based filesystem image generator for type II filesystem
                    (LAT-32)

  FixDir/         - PC-based direcotry adjuster for prior Elf/OS versions.

  kernel.118      - build 118 of Elf/OS 5 kernel

Notes:

  eosinst1.intel and eosinst2.intel do not include BIOS images.  before
burning these installers on a ROM they need to be combined with a valid BIOS.

  eosinst1.intel and eosinst2.intel are expected to be jumped to on boot, as
such they require <ENTER> to be pressed for the auto-buad detection for
terminal I/O, and expect to be jumped to with P=0.

  eosinst1r.intel and eosinst2r.intel expect the auto-baud to already be set
and therefore no <ENTER> press is needed.  They also expect to be jumped to
with P=3.  These two installers also expect SCAL/SRET to already be setup.

  FsGen1 and FsGen2 are PC-based c programs that can create Elf/OS v5 disk
images that are suitable for use with an emulator or can be burned onto a
CF/SD card if you have the appropriate hardware/software to do so.  Both of
these programs can create a disk image of any size.  Provide the size in mb
as a command line argument, for example, if you want to create a 1gb Type-II
disk, then use 'fsgen2 1000'.

  The directory structure in Elf/OS v5 is slightly modified from prior
versions.  As such the kernel included with the installers should not be
used on an existing Elf/OS disk.  Filesystems for older Elf/OS versions
need to have the directories adjusted before installing Elf/OS v5.  The
FixDir program is a PC-based tool for fixing the directories of filesystem
image files.  An 1802-based version of the DirFix tool is being built.

  dir, chmod, and touch have been modified to work with the new Elf/OS v5
directories, when upgrading an exising Elf/OS system to v5, these three
files should be updated before booting on the v5 kernel.

  The prior 'disktool' program does not work correctly with Elf/OS v5
directories, either Type-I or Type-II, as such do not use the old disktool
program on Elf/OS v5 disk images, corruption will result. A new version
of disktool that can work with v5 images will be released soon.

  No BIOS alterations were made to support Elf/OS v5, as such any BIOS
version that could run Elf/OS4 should be find for running Elf/OS v5.  All
of my testing has been done with BIOS 1.0.12.


# OPPO R11 系列扩容分区工具箱

⚠️本程序是本人早期~~拉的史~~，代码质量及其低下，同时本人目前已没有时间将其重构，请酌情使用⚠️

这是一个基于 **parted** 的 OPPO R11 系列扩容分区工具箱

## 程序主菜单

```
=================================================
          欢迎使用R11系列 Parted扩容工具箱       
                   作者:WenHao              
             未经允许，禁止用于付费远程        
=================================================
          请确保您的TWRP版本为3.6.x/3.7.x       
=================================================

- 01.重启功能区
- 02.扩容功能区
- 03.清除功能区
- 04.128GB主板专区
- xx.xxxxxxxxxx

-------------------------------------------------
- 0.退出                           版本:x.x
=================================================
请输入你想要使用的功能序号:
```

## 构建

使用 gcc

```
gcc OPPO_R11_Series_Parted_ToolBox.c -o OPPO_R11_Series_Parted_ToolBox.exe
```

使用 clang

```
clang OPPO_R11_Series_Parted_ToolBox.c -o OPPO_R11_Series_Parted_ToolBox.exe
```

你还需要下载并解压 [**platfrom-tools**](https://developer.android.google.cn/studio/releases/platform-tools?hl=zh-cn) ，同时拥有 **parted**(aarch64) 二进制文件，并将他们放置在同一个文件夹中

## 其他信息

64GB 手机原始 EMMC 分区表信息

```
Model: MMC xxxxxx (sd/mmc)
Disk /dev/block/mmcblk0: 62.5GB
Sector size (logical/physical): 512B/512B
Partition Table: gpt
Disk Flags:

Number  Start   End     Size    File system  Name           Flags
 1      20.5kB  10.5MB  10.5MB               oppodycnvbk
 2      10.5MB  21.0MB  10.5MB               oppostanvbk
 3      21.0MB  23.1MB  2097kB               modemst1
 4      23.1MB  25.2MB  2097kB               modemst2
 5      25.2MB  58.7MB  33.6MB  ext4         persist
 6      58.7MB  60.8MB  2097kB               reserve1
 7      60.8MB  77.6MB  16.8MB               reserve2
 8      77.6MB  79.7MB  2097kB               reserve3
 9      79.7MB  96.5MB  16.8MB               reserve4
10      101MB   104MB   3670kB               xbl
11      104MB   108MB   3670kB               xblbak
12      108MB   112MB   4194kB               tz
13      112MB   116MB   4194kB               tzbak
14      116MB   117MB   524kB                rpm
15      117MB   117MB   524kB                rpmbak
16      117MB   118MB   524kB                hyp
17      118MB   118MB   524kB                hypbak
18      118MB   119MB   524kB                pmic
19      119MB   120MB   524kB                pmicbak
20      120MB   122MB   2097kB               fsg
21      126MB   193MB   67.1MB               boot
22      193MB   260MB   67.1MB               bootbak
23      260MB   327MB   67.1MB               recovery
24      327MB   328MB   1049kB               keymaster
25      328MB   329MB   1049kB               keymasterbak
26      329MB   330MB   1049kB               cmnlib
27      330MB   331MB   1049kB               cmnlib64
28      331MB   332MB   1049kB               cmnlibbak
29      332MB   333MB   1049kB               cmnlib64bak
30      333MB   338MB   4194kB               mdtpsecapp
31      338MB   342MB   4194kB               mdtpsecappbak
32      342MB   375MB   33.6MB               mdtp
33      375MB   409MB   33.6MB               mdtpbak
34      409MB   426MB   16.8MB  ext4         dsp
35      426MB   442MB   16.8MB  ext4         dspbak
36      442MB   444MB   1049kB               abl
37      444MB   445MB   1049kB               ablbak
38      445MB   446MB   1049kB               dip
39      446MB   446MB   1024B                devinfo
40      446MB   446MB   262kB                apdp
41      446MB   446MB   262kB                msadp
42      446MB   446MB   1024B                dpo
43      446MB   480MB   34.2MB               splash
44      480MB   480MB   4096B                limits
45      480MB   481MB   1049kB               toolsfv
46      487MB   495MB   8389kB               logfs
47      495MB   496MB   1049kB               ddr
48      496MB   496MB   16.4kB               sec
49      496MB   497MB   1049kB               bluetooth
50      497MB   498MB   1049kB               bluetoothbak
51      503MB   503MB   1024B                fsc
52      503MB   503MB   8192B                ssd
53      503MB   504MB   1049kB               misc
54      504MB   505MB   524kB                keystore
55      505MB   506MB   1049kB               devcfg
56      506MB   507MB   1049kB               devcfgbak
57      507MB   508MB   524kB                frp
58      508MB   575MB   67.1MB               logdump
59      575MB   577MB   2097kB               sti
60      577MB   594MB   16.8MB               LOGO
61      594MB   627MB   33.6MB               DRIVER
62      629MB   629MB   65.5kB               vbmeta
63      629MB   629MB   65.5kB               vbmetabak
64      629MB   763MB   134MB   fat16        modem          msftdata
65      772MB   1040MB  268MB   ext4         cache
66      1040MB  4521MB  3481MB  ext2         system
67      4521MB  5595MB  1074MB  ext2         vendor
68      5595MB  5595MB  131kB                storsec
69      5604MB  62.5GB  56.9GB               userdata
```

128GB 手机原始 EMMC 分区表信息

```
Model: MMC xxxxxx (sd/mmc)
Disk /dev/block/mmcblk0: 125GB
Sector size (logical/physical): 512B/512B
Partition Table: gpt
Disk Flags: 

Number  Start   End     Size    File system  Name           Flags
 1      20.5kB  10.5MB  10.5MB               oppodycnvbk
 2      10.5MB  21.0MB  10.5MB               oppostanvbk
 3      21.0MB  23.1MB  2097kB               modemst1
 4      23.1MB  25.2MB  2097kB               modemst2
 5      25.2MB  58.7MB  33.6MB  ext4         persist
 6      58.7MB  60.8MB  2097kB               reserve1
 7      60.8MB  77.6MB  16.8MB               reserve2
 8      77.6MB  79.7MB  2097kB               reserve3
 9      79.7MB  96.5MB  16.8MB               reserve4
10      101MB   104MB   3670kB               xbl
11      104MB   108MB   3670kB               xblbak
12      108MB   112MB   4194kB               tz
13      112MB   116MB   4194kB               tzbak
14      116MB   117MB   524kB                rpm
15      117MB   117MB   524kB                rpmbak
16      117MB   118MB   524kB                hyp
17      118MB   118MB   524kB                hypbak
18      118MB   119MB   524kB                pmic
19      119MB   120MB   524kB                pmicbak
20      120MB   122MB   2097kB               fsg
21      126MB   193MB   67.1MB               boot
22      193MB   260MB   67.1MB               bootbak
23      260MB   327MB   67.1MB               recovery
24      327MB   328MB   1049kB               keymaster
25      328MB   329MB   1049kB               keymasterbak
26      329MB   330MB   1049kB               cmnlib
27      330MB   331MB   1049kB               cmnlib64
28      331MB   332MB   1049kB               cmnlibbak
29      332MB   333MB   1049kB               cmnlib64bak
30      333MB   338MB   4194kB               mdtpsecapp
31      338MB   342MB   4194kB               mdtpsecappbak
32      342MB   375MB   33.6MB               mdtp
33      375MB   409MB   33.6MB               mdtpbak
34      409MB   426MB   16.8MB  ext4         dsp
35      426MB   442MB   16.8MB  ext4         dspbak
36      442MB   444MB   1049kB               abl
37      444MB   445MB   1049kB               ablbak
38      445MB   446MB   1049kB               dip
39      446MB   446MB   1024B                devinfo
40      446MB   446MB   262kB                apdp
41      446MB   446MB   262kB                msadp
42      446MB   446MB   1024B                dpo
43      446MB   480MB   34.2MB               splash
44      480MB   480MB   4096B                limits
45      480MB   481MB   1049kB               toolsfv
46      487MB   495MB   8389kB               logfs
47      495MB   496MB   1049kB               ddr
48      496MB   496MB   16.4kB               sec
49      496MB   497MB   1049kB               bluetooth
50      497MB   498MB   1049kB               bluetoothbak
51      503MB   503MB   1024B                fsc
52      503MB   503MB   8192B                ssd
53      503MB   504MB   1049kB               misc
54      504MB   505MB   524kB                keystore
55      505MB   506MB   1049kB               devcfg
56      506MB   507MB   1049kB               devcfgbak
57      507MB   508MB   524kB                frp
58      508MB   575MB   67.1MB               logdump
59      575MB   577MB   2097kB               sti
60      577MB   594MB   16.8MB               LOGO
61      594MB   627MB   33.6MB               DRIVER
62      629MB   629MB   65.5kB               vbmeta
63      629MB   629MB   65.5kB               vbmetabak
64      629MB   763MB   134MB   fat16        modem          msftdata
65      772MB   1040MB  268MB   ext4         cache
66      1040MB  4521MB  3481MB  ext4         system
67      4521MB  5595MB  1074MB  ext4         vendor
68      5595MB  5595MB  131kB                storsec
69      5604MB  125GB   119GB                userdata
```

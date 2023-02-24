#include <stdio.h>
#include <stdlib.h>
int menu(void); //菜单
void bar(void); //分割栏
void safe_flush(FILE *fp); //清除scanf缓存(别看了,抄的)
int main(void)
{
    int int_system; //用户输入的system大小
    int new_system; //新的system结束位置
    int int_vendor; //用户输入的vendor大小
    int new_vendor; //新的vendor开始位置
    int new_vendore; //新的vendor结束位置
    int new_userdata; //新的userdata开始位置
    char mkpart_system[70]; //用于存放新建system命令的变量
    char mkpart_vendor[70]; //用于存放新建vendor命令的变量
    char mkpart_userdata[70]; //用于存放新建userdata命令的变量
    char char_choose; //存放用户的决定
    bar();
    printf("结束系统内原先存在的adb进程...\n");
    system("taskkill /f /im adb.exe"); //结束adb进程，防止干扰下一步程序运行
    bar();
    printf("启动adb服务...\n");
    system("adb start-server");
    bar();
    system("CLS"); //清屏
    while (1)
    {
        int_system = 3320; //防止部分步骤被用户误操作，给予初始值
        new_system = 8925; 
        int_vendor = 1024; 
        new_vendor = 8926; 
        new_vendore = 9950; 
        new_userdata = 9951; 
        switch (menu()) //对应功能
        {
            case 0:
                exit(0);
            case 1: //Fastboot重启到Recovery
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("echo boot-recovery > misc.bin"); //创建misc文件
                system("fastboot flash misc misc.bin");
                system("del misc.bin");
                printf("设备已连接!   状态:Fastboot\n");
                system("fastboot reboot");
                printf("好啦！\n");
                bar();
                break;
            case 2: //开机状态重启到Recovery
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-device devices");
                printf("设备已连接!   状态:System\n");
                system("adb reboot recovery");
                printf("好啦！\n");
                bar();
                break;
            case 3: //只扩容System分区
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb push parted /sbin/parted");
                printf("Parted已经推送过去啦!进行下一步操作\n");
                system("adb root");
                system("adb shell chmod 0755 /sbin/parted");
                printf("已经把Parted二进制文件赋予755权限啦!\n");
                while (1)
                {
                    printf("请输入您要扩容的System分区大小(单位为MB,范围为3320~10240):"); //限定范围，防止误操作
                    scanf("%d", &int_system);
                    safe_flush(stdin); //清除scanf缓存
                    if (int_system <= 10240 && int_system >= 3320)
                        break;
                    else
                    {
                        printf("您输入的数据过大或过小，出于保护，请您重新输入System分区大小\n");
                        continue;
                    }
                }
                new_system = 5605 + int_system; //计算新的system分区结束位置
                new_userdata = new_system + 1; //计算新的userdata分区的起始位置
                while (1)
                {
                    printf("此工具箱仅适用于未使用过扩容包或者使用本工具箱和按照作者写的扩容教程扩容的手机\n");
                    printf("忽略此话导致的扩容失败变砖WenHao概不负责\n");
                    printf("扩容分区可能会导致你的手机变砖，确认要继续操作吗？(y/N)");
                    scanf("%c", &char_choose);
                    safe_flush(stdin); //清除scanf缓存
                    if (char_choose == 'Y' || char_choose == 'y') //判断用户的输入是否合法
                        break;
                    else
                        exit(0);
                }
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                system("adb shell twrp umount data");
                printf("删除System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 66");
                printf("删除Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 69");
                printf("建立新的System分区...\n");
                sprintf(mkpart_system, "adb shell parted /dev/block/mmcblk0 mkpart -s system EXT4 5605 %d", new_system); //合并命令
                system(mkpart_system);
                printf("建立新的Userdata分区...\n");
                sprintf(mkpart_userdata, "adb shell parted /dev/block/mmcblk0 mkpart -s userdata EXT4 %d 62.5g", new_userdata); //合并命令
                system(mkpart_userdata);
                printf("扩容完成\n");
                printf("请重启设备...\n");
                printf("扩容完成~\n");
                bar();
                break;
            case 4: //同时扩容System与Vendor分区
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb push parted /sbin/parted");
                printf("Parted已经推送过去啦!进行下一步操作\n");
                system("adb root");
                system("adb shell chmod 0755 /sbin/parted");
                printf("已经把Parted二进制文件赋予755权限啦!\n");
                while (1)
                {
                    printf("此工具箱仅适用于未使用过扩容包或者使用本工具箱和按照作者写的扩容教程扩容的手机\n");
                    printf("忽略此话导致的扩容失败变砖WenHao概不负责\n");
                    printf("请输入您要扩容的System分区大小(单位为MB,范围为3320~10240):"); //限定范围，防止误操作
                    scanf("%d", &int_system);
                    safe_flush(stdin); //清除scanf缓存
                    if (int_system <= 10240 && int_system >= 3320)
                        break;
                    else
                    {
                        printf("您输入的数据过大或过小，出于保护，请您重新输入System分区大小\n");
                        continue;
                    }
                }
                while (1)
                {
                    printf("请输入您要扩容的Vendor分区大小(单位为MB,范围为1024~3072):"); //限定范围，防止误操作
                    scanf("%d", &int_vendor);
                    safe_flush(stdin); //清除scanf缓存
                    if (int_vendor <= 3072 && int_vendor >= 1024)
                        break;
                    else
                    {
                        printf("您输入的数据过大或过小，出于保护，请您重新输入Vendor分区大小\n");
                        continue;
                    }
                }
                new_system = 5605 + int_system; //计算新的system分区结束位置
                new_vendor = new_system + 1; //计算新的vendor起始位置
                new_vendore = new_vendor + int_vendor; //计算新的vendor结束位置
                new_userdata = new_vendore + 1; //计算新的userdata分区的起始位置
                while (1)
                {
                    printf("此工具箱仅适用于未使用过扩容包或者使用本工具箱和按照作者写的扩容教程扩容的手机\n");
                    printf("忽略此话导致的扩容失败变砖WenHao概不负责\n");
                    printf("扩容分区可能会导致你的手机变砖，确认要继续操作吗？(y/N)");
                    scanf("%c", &char_choose);
                    safe_flush(stdin); //清除scanf缓存
                    if (char_choose == 'Y' || char_choose == 'y') //判断用户的输入是否合法
                        break;
                    else
                        exit(0);
                }
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                system("adb shell twrp umount vendor");
                system("adb shell twrp umount data");
                printf("删除System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 66");
                printf("删除Vendor分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 67");
                printf("删除Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 69");
                printf("建立新的System分区...\n");
                sprintf(mkpart_system, "adb shell parted /dev/block/mmcblk0 mkpart -s system EXT4 5605 %d", new_system); //合并命令
                system(mkpart_system);
                printf("建立新的Vendor分区...\n");
                sprintf(mkpart_vendor, "adb shell parted /dev/block/mmcblk0 mkpart -s vendor EXT4 %d %d", new_vendor, new_vendore); //合并命令
                system(mkpart_vendor);
                printf("建立新的Userdata分区...\n");
                sprintf(mkpart_userdata, "adb shell parted /dev/block/mmcblk0 mkpart -s userdata EXT4 %d 62.5g", new_userdata); //合并命令
                system(mkpart_userdata);
                printf("扩容完成\n");
                printf("请重启设备...\n");
                printf("扩容完成~\n");
                bar();
                break;
            case 5: //扩容System为5GB、Vendor分区为2GB
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb push parted /sbin/parted");
                printf("Parted已经推送过去啦!进行下一步操作\n");
                system("adb root");
                system("adb shell chmod 0755 /sbin/parted");
                printf("已经把Parted二进制文件赋予755权限啦!\n");
                while (1)
                {
                    printf("此工具箱仅适用于未使用过扩容包或者使用本工具箱和按照作者写的扩容教程扩容的手机\n");
                    printf("忽略此话导致的扩容失败变砖WenHao概不负责\n");
                    printf("扩容分区可能会导致你的手机变砖，确认要继续操作吗？(y/N)");
                    scanf("%c", &char_choose);
                    safe_flush(stdin); //清除scanf缓存
                    if (char_choose == 'Y' || char_choose == 'y') //判断用户的输入是否合法
                        break;
                    else
                        exit(0);
                }
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                system("adb shell twrp umount vendor");
                system("adb shell twrp umount data");
                printf("删除System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 66");
                printf("删除Vendor分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 67");
                printf("删除Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 69");
                printf("建立新的System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s system EXT4 5605 10725");
                printf("建立新的Vendor分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s vendor EXT4 10726 12774");
                printf("建立新的Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s userdata EXT4 12775 62.5g");
                printf("扩容完成\n");
                printf("请重启设备...\n");
                printf("扩容完成~\n");
                bar();
                break;
            case 6: //还原原分区
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb push parted /sbin/parted");
                printf("Parted已经推送过去啦!进行下一步操作\n");
                system("adb root");
                system("adb shell chmod 0755 /sbin/parted");
                printf("已经把Parted二进制文件赋予755权限啦!\n");
                while (1)
                {
                    printf("此工具箱仅适用于未使用过扩容包或者使用本工具箱和按照作者写的扩容教程扩容的手机\n");
                    printf("忽略此话导致的扩容失败变砖WenHao概不负责\n");
                    printf("恢复分区可能会导致你的手机变砖，确认要继续操作吗？(y/N)");
                    scanf("%c", &char_choose);
                    safe_flush(stdin); //清除scanf缓存
                    if (char_choose == 'Y' || char_choose == 'y') //判断用户的输入是否合法
                        break;
                    else
                        exit(0);
                }
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                system("adb shell twrp umount vendor");
                system("adb shell twrp umount data");
                printf("删除System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 66");
                printf("删除Vendor分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 67");
                printf("删除Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 rm -s 69");
                printf("建立新的System分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s system EXT4 1040 4521");
                printf("建立新的Vendor分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s vendor EXT4 4521 5595");
                printf("建立新的Userdata分区...\n");
                system("adb shell parted /dev/block/mmcblk0 mkpart -s userdata EXT4 5604 62.5g");
                printf("扩容完成\n");
                printf("重启设备...\n");
                system("adb reboot recovery");
                printf("已经发送重启指令了!请在重启后进入\"清除-格式化Data\"格式化Data分区，格式化完成以后进入\"清除-高级清除\"勾选System和Vendor分区并滑动清除!\n");
                printf("等待设备重启成功\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                printf("执行格式化...");
                system("adb shell twrp wipe system");
                system("adb shell twrp wipe vendor");
                system("adb shell twrp format data");
                printf("恢复分区完成~\n");
                bar();
                break;
            case 7: //获取手机当前分区表数据
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb push parted /sbin/parted");
                printf("Parted已经推送过去啦!进行下一步操作\n");
                system("adb root");
                system("adb shell chmod 0755 /sbin/parted");
                printf("已经把Parted二进制文件赋予755权限啦!\n");
                system("adb shell parted /dev/block/mmcblk0 print > PartitionTable.txt");
                printf("已成功将手机当前分区表数据输出至PartitionTable.txt,请查阅!\n");
                bar();
                break;
            case 8: //TWRP自带格式化System分区(需要TWRP支持)
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                printf("开始格式化System分区\n");
                system("adb shell twrp wipe system");
                printf("格式化System分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 9: //格式化System分区
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount system");
                printf("开始格式化System分区\n");
                system("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/system");
                printf("格式化System分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 10: //TWRP自带格式化Vendor分区(需要TWRP支持)
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount vendor");
                printf("开始格式化Vendor分区\n");
                system("adb shell twrp wipe vendor");
                printf("格式化Vendor分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 11: //格式化Vendor分区
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount vendor");
                printf("开始格式化Vendor分区\n");
                system("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/vendor");
                printf("格式化Vendor分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 12: //通用格式化Data分区(需要TWRP支持)
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system("adb shell twrp format data");
                printf("格式化Data分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 13: //格式化Data分区_R11
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system("adb shell mke2fs -t ext4 /dev/block/bootdevice/by-name/userdata");
                printf("格式化Data分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            case 14: //格式化Data分区_R11s
                bar();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar();
                printf("解除挂载...\n");
                system("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system("adb shell make_f2fs /dev/block/bootdevice/by-name/userdata");
                printf("格式化Data分区完成\n");
                printf("请重启设备...\n");
                printf("完成~\n");
                bar();
                break;
            default: //去除错误输入
                system("CLS"); //清屏
                bar();
                printf("\n");
                printf("\n");
                printf("         您输入的数值有误，请重新输入!        \n");
                printf("\n");
                printf("\n");
                bar();
                safe_flush(stdin); //清除scanf缓存
                getchar();
                system("CLS"); //清屏
                continue;
        }
        printf("任务执行完成!\n");
        printf("按回车键返回主菜单");
        getchar();
        system("CLS"); //清屏
    }
    return 0;
}
void bar(void)
{
   printf("===========================================\n"); 
}
int menu(void)
{
    int int_choose;
    bar();
    printf("       欢迎使用R11系列 Parted扩容工具箱       \n");
    printf("               作者:WenHao              \n");
    printf("          未经允许，禁止用于付费远程        \n");
    bar();
    printf("     请确保您的TWRP版本为3.6.2/3.7.0       \n");
    bar();
    printf("             ==重启功能区==              \n");
    printf("- 1.Fastboot重启到Recovery              \n");
    printf("- 2.开机状态重启到Recovery                \n");
    printf("             ==扩容功能区==              \n");
    printf("- 3.只扩容System分区                     \n");
    printf("- 4.同时扩容System与Vendor分区           \n");
    printf("- 5.扩容System为5GB、Vendor分区为2GB     \n");
    printf("- 6.还原原分区表                         \n");
    printf("- 7.获取手机当前分区表数据                 \n");
    printf("             ==清除功能区==              \n");
    printf("- 8.TWRP自带格式化System分区(需要TWRP支持)\n");
    printf("- 9.格式化System分区                    \n");
    printf("- 10.TWRP自带格式化Vendor分区(需要TWRP支持)\n");
    printf("- 11.格式化Vendor分区                    \n");
    printf("- 12.通用格式化Data分区(需要TWRP支持)       \n");
    printf("- 13.格式化Data分区_R11                  \n");
    printf("- 14.格式化Data分区_R11s                \n");
    bar();
    printf("- 0.退出                                   \n");
    bar();
    while (1)
    {
        printf("请输入你想要使用的功能序号：");
        if (scanf("%d", &int_choose) == 1)
        {
            safe_flush(stdin); //清除scanf缓存
            system("CLS"); //清屏
            break;
        }
        else
        {
            system("CLS"); //清屏
            return -1; //去除错误输入
        }
    }
    return int_choose;
}
void safe_flush(FILE *fp)
{
 int ch;
 while( (ch = fgetc(fp)) != EOF && ch != '\n' );     
}
#include <stdio.h>
#include <stdlib.h>
#define VERSION "2.1"
void system_plus(const char *command); //基于system函数,但是加了命令是否执行成功判断
void bar1(void); //分隔栏1(=====)
void bar2(void); //分隔栏2(-----)
void confirm_operation(void); //询问用户是否继续操作
void safe_flush(FILE *fp); //清除stdin函数(抄的)
void init(void); //检测设备连接状态并推送parted
void umount_partation(void); //解除挂载System,Vendor,Data分区
void parted_rm_partition1(void); //删除System,Data分区
void parted_rm_partition2(void); //删除System,Vendor,Data分区
int menu(void); //菜单
int enter_system_partation_size(void); //输入System分区大小函数
int enter_vendor_partation_size(void); //输入Vendor分区大小函数
int main(void)
{
	const int system_partation_start = 5604;
	int system_partation_end;
	int vendor_partation_start;
	int vendor_partation_end;
	int userdata_partation_start;
	int partation_size;
	char mkpart_command[70];
	bar1();
    printf("结束系统内原先存在的adb进程...\n");
    system("taskkill /f /im adb.exe"); //结束adb进程,防止干扰下一步程序运行
    bar1();
    printf("启动adb服务...\n");
    system("adb start-server");
    bar1();
    system("CLS"); //清屏
	bar1();
	printf("                      注意事项                   \n");
	bar2();
	printf("1.在使用本工具前请确保您的TWRP版本为3.6.x/3.7.x  \n");
	printf("2.请确保您的手机未使用过扩容包进行扩容           \n");
	bar1();
	printf("请按回车键继续");
	getchar();
	system("CLS"); //清屏
	while (1)
	{
		switch (menu())
		{
			case -3: //返回主菜单
				system("CLS"); //清屏
				continue;
			case -2: //退出程序
				exit(0);
			case -1: //去除错误输入
				system("CLS"); //清屏
				bar1();
				printf("\n");
				printf("\n");
				printf("          您输入的数值有误，请重新输入!        \n");
				printf("\n");
				printf("\n");
				bar1();
				break;
			case 1: //Fastboot重启到Recovery
                bar1();
                printf("正在检测设备连接状态,如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("echo boot-recovery > misc.bin"); //创建misc文件
                system_plus("fastboot flash misc misc.bin");
                system_plus("del misc.bin"); //清理文件
                printf("设备已连接!   状态:Fastboot\n");
                system_plus("fastboot reboot");
                bar1();
                break;
            case 2: //开机状态重启到Recovery
                bar1();
                printf("正在检测设备连接状态,如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-device devices");
                printf("设备已连接!   状态:System\n");
                system_plus("adb reboot recovery");
                bar1();
                break;
			case 3: //只扩容System分区 
				init();
				partation_size = enter_system_partation_size();
				system_partation_end = system_partation_start + partation_size;
				userdata_partation_start = system_partation_end;
				confirm_operation();
				umount_partation();
				parted_rm_partition1();
				printf("建立新的System分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partation_start, system_partation_end);
				system_plus(mkpart_command); //建立System分区
				printf("建立新的Userdata分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d 62.5g", userdata_partation_start);
				system_plus(mkpart_command); //建立Data分区
				break;
			case 4: //同时扩容System与Vendor分区
				init();
				partation_size = enter_system_partation_size();
				system_partation_end = system_partation_start + partation_size;
				vendor_partation_start = system_partation_end;
				partation_size = enter_vendor_partation_size();
				vendor_partation_end = vendor_partation_start + partation_size;
				userdata_partation_start = vendor_partation_end;
				confirm_operation();
				umount_partation();
				parted_rm_partition2();
				printf("建立新的System分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partation_start, system_partation_end);
				system_plus(mkpart_command); //建立System分区
				printf("建立新的Vendor分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partation_start, vendor_partation_end);
				system_plus(mkpart_command); //建立Vendor分区
				printf("建立新的Userdata分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d 62.5g", userdata_partation_start);
				system_plus(mkpart_command); //建立Data分区
				break;
			case 5: //扩容System为5GB、Vendor分区为2GB
				init();
				confirm_operation();
				umount_partation();
				parted_rm_partition2();
                printf("建立新的System分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 5605 10725");
                printf("建立新的Vendor分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 10726 12774");
                printf("建立新的Userdata分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 12775 62.5g");
                printf("扩容完成\n");
                printf("请重启设备...\n");
				break;
			case 6: //还原原分区表
				init();
				confirm_operation();
				parted_rm_partition2();
				printf("恢复System分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
                printf("恢复Vendor分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
                printf("恢复Userdata分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 62.5g");
				printf("恢复完成\n");
				break;
			case 7: //获取手机当前分区表数据
				init();
				system_plus("adb shell parted /dev/block/mmcblk0 print > PartitionTable.txt");
                printf("已成功将手机当前分区表数据输出至PartitionTable.txt,请查阅!\n");
				break;
			case 8: //TWRP自带格式化System分区(需要TWRP支持)
				bar1();
                printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount system");
                printf("开始格式化System分区\n");
                system_plus("adb shell twrp wipe system");
                printf("格式化System分区完成\n");
				break;
			case 9: //格式化System分区
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount system");
                printf("开始格式化System分区\n");
                system_plus("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/system");
                printf("格式化System分区完成\n");
				break;
			case 10: //TWRP自带格式化Vendor分区(需要TWRP支持)
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount vendor");
                printf("开始格式化Vendor分区\n");
                system_plus("adb shell twrp wipe vendor");
                printf("格式化Vendor分区完成\n");
				break;
			case 11: //格式化Vendor分区
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount vendor");
                printf("开始格式化Vendor分区\n");
                system_plus("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/vendor");
                printf("格式化Vendor分区完成\n");
				break;
			case 12: //通用格式化Data分区(需要TWRP支持)
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system_plus("adb shell twrp format data");
                printf("格式化Data分区完成\n");
				break;
			case 13: //格式化Data分区_R11
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system_plus("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system_plus("adb root");
                bar2();
                printf("解除挂载...\n");
                system_plus("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system_plus("adb shell mke2fs -t ext4 /dev/block/bootdevice/by-name/userdata");
                printf("格式化Data分区完成\n");
				break;
			case 14: //格式化Data分区_R11s
				bar1();
				printf("正在检测设备连接状态，如果长时间卡在此处请检查设备及设备驱动程序\n");
                system("adb wait-for-recovery devices");
                printf("设备已连接!   状态:Recovery\n");
                system("adb root");
                bar2();
                printf("解除挂载...\n");
                system("adb shell twrp umount data");
                printf("开始格式化Data分区\n");
                system("adb shell make_f2fs /dev/block/bootdevice/by-name/userdata");
                printf("格式化Data分区完成\n");
				break;
			case 15: //128G 只扩容System分区
				init();
				partation_size = enter_system_partation_size();
				system_partation_end = system_partation_start + partation_size;
				userdata_partation_start = system_partation_end;
				confirm_operation();
				umount_partation();
				parted_rm_partition1();
				printf("建立新的System分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partation_start, system_partation_end);
				system_plus(mkpart_command); //建立System分区
				printf("建立新的Userdata分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d 125g", userdata_partation_start);
				system_plus(mkpart_command); //建立Data分区
				break;
			case 16: //128G 同时扩容System与Vendor分区
				init();
				partation_size = enter_system_partation_size();
				system_partation_end = system_partation_start + partation_size;
				vendor_partation_start = system_partation_end;
				partation_size = enter_vendor_partation_size();
				vendor_partation_end = vendor_partation_start + partation_size;
				userdata_partation_start = vendor_partation_end;
				confirm_operation();
				umount_partation();
				parted_rm_partition2();
				printf("建立新的System分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partation_start, system_partation_end);
				system_plus(mkpart_command); //建立System分区
				printf("建立新的Vendor分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partation_start, vendor_partation_end);
				system_plus(mkpart_command); //建立Vendor分区
				printf("建立新的Userdata分区...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d 125g", userdata_partation_start);
				system_plus(mkpart_command); //建立Data分区
				break;
			case 17: //128G 扩容System为5GB、Vendor分区为2GB
				init();
				confirm_operation();
				umount_partation();
				parted_rm_partition2();
				printf("建立新的System分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 5605 10725");
                printf("建立新的Vendor分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 10726 12774");
                printf("建立新的Userdata分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 12775 125g");
                printf("扩容完成\n");
				break;
			case 18: //128G 还原原分区表
				init();
				confirm_operation();
				umount_partation();
				parted_rm_partition2();
				printf("恢复System分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
                printf("恢复Vendor分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
                printf("恢复Userdata分区...\n");
                system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 125g");
				printf("恢复完成\n");
				break;
		}
		bar1();
		printf("按回车键返回主菜单");
		getchar();
		system("CLS"); //清屏
	}
	return 0;
}
void system_plus(const char *command)
{
    char choose;
    if (system(command) != 0)
    {
        printf("命令可能执行失败,您可以继续操作,但造成的后果作者不予承担\n");
        printf("是否继续操作(y/N)");
        if (scanf("%c", &choose) == 1)
        {
            safe_flush(stdin);
            if (choose == 'y' || choose == 'Y')
                return;
            else
                exit(0);
        }
        else
            exit(0);
    }
    else
        return;
}
void bar1(void)
{
	printf("=================================================\n");
}
void bar2(void)
{
	printf("-------------------------------------------------\n");
}
void confirm_operation(void)
{
	char choose;
	printf("此操作可能会导致你的手机变砖,确认要继续操作吗?(y/N)");
    scanf("%c", &choose);
    safe_flush(stdin); //清除scanf缓存
    if (choose == 'Y' || choose == 'y') //判断用户的输入是否合法
        return;
    else
        exit(0);
}
void safe_flush(FILE *fp)
{
	int ch;
	while( (ch = fgetc(fp)) != EOF && ch != '\n' );     
}
void init(void)
{
	system("CLS"); //清屏
	bar1();
    printf("正在检测设备连接状态,如果长时间卡在此处请检查设备及设备驱动程序\n");
    system_plus("adb wait-for-recovery devices");
    printf("设备已连接!   状态:Recovery\n");
    system_plus("adb push parted /sbin/parted");
    printf("Parted已经推送过去啦!进行下一步操作\n");
    system_plus("adb root");
    system_plus("adb shell chmod 0755 /sbin/parted");
    printf("已经把Parted二进制文件赋予755权限啦!\n");
	bar2();
}
void umount_partation(void)
{
	printf("解除挂载...\n");
    system_plus("adb shell twrp umount system");
    system_plus("adb shell twrp umount vendor");
    system_plus("adb shell twrp umount data");
	bar2();
}
void parted_rm_partition1(void)
{
	printf("删除System分区...\n");
    system_plus("adb shell parted /dev/block/mmcblk0 rm 66");
    printf("删除Userdata分区...\n");
    system_plus("adb shell parted /dev/block/mmcblk0 rm 69");
	bar2();
}
void parted_rm_partition2(void)
{
	printf("删除System分区...\n");
    system_plus("adb shell parted /dev/block/mmcblk0 rm 66");
    printf("删除Vendor分区...\n");
    system_plus("adb shell parted /dev/block/mmcblk0 rm 67");
    printf("删除Userdata分区...\n");
    system_plus("adb shell parted /dev/block/mmcblk0 rm 69");
	bar2();
}
int menu(void)
{
	int choose1;
	int choose2;
	bar1();
	printf("       欢迎使用R11系列 Parted扩容工具箱       \n");
    printf("                 作者:WenHao              \n");
    printf("          未经允许，禁止用于付费远程        \n");
    bar1();
    printf("     请确保您的TWRP版本为3.6.x/3.7.x       \n");
    bar1();
	printf("\n");
	printf("- 01.重启功能区\n");
	printf("- 02.扩容功能区\n");
	printf("- 03.清除功能区\n");
	printf("- 04.128GB主板专区\n");
	printf("\n");
	bar2();
	printf("- 0.退出                           版本:%s\n", VERSION);
	bar1();
	 while (1)
    {
        printf("请输入你想要使用的功能序号:");
        if (scanf("%d", &choose1) == 1)
        {
			safe_flush(stdin); //清除stdin
			system("CLS"); //清屏
			switch (choose1) //二级菜单
			{
				case 0:
					return -2; //返回值-2,由主函数退出程序
				case 1:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==重启功能区==              \n");
					printf("\n");
					printf("- 01.Fastboot重启到Recovery              \n");
					printf("- 02.开机状态重启到Recovery                \n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.返回主菜单                                   \n");
					bar1();
					printf("请输入你想要使用的功能序号:");
					if (scanf("%d", &choose2) == 1)
					{
						safe_flush(stdin); //清除stdin
						switch(choose2)
						{
							case 0:
								return -3; //由主函数返回主菜单
							case 1:
								return choose2; //主函数对应功能
							case 2:
								return choose2; //主函数对应功能
							default:
								return -1; //返回值-1，由主函数去除错误输入
						}
					}
					else
						return -1; //返回值-1，由主函数去除错误输入
				case 2:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==扩容功能区==              \n");
					printf("\n");
					printf("- 03.只扩容System分区                     \n");
					printf("- 04.同时扩容System与Vendor分区           \n");
					printf("- 05.扩容System为5GB、Vendor分区为2GB     \n");
					printf("- 06.还原原分区表                         \n");
					printf("- 07.获取手机当前分区表数据               \n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.返回主菜单                                   \n");
					bar1();
					printf("请输入你想要使用的功能序号:");
					if (scanf("%d", &choose2) == 1)
					{
						safe_flush(stdin); //清除stdin
						switch(choose2)
						{
							case 0:
								return -3; //由主函数返回主菜单
							case 3:
								return choose2; //主函数对应功能
							case 4:
								return choose2; //主函数对应功能
							case 5:
								return choose2; //主函数对应功能
							case 6:
								return choose2; //主函数对应功能
							case 7:
								return choose2; //主函数对应功能
							default:
								return -1; //返回值-1，由主函数去除错误输入
						}
					}
					else
						return -1; //返回值-1，由主函数去除错误输入
				case 3:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==清除功能区==              \n");
					printf("\n");
					printf("- 08.TWRP自带格式化System分区(需要TWRP支持)\n");
					printf("- 09.格式化System分区                    \n");
					printf("- 10.TWRP自带格式化Vendor分区(需要TWRP支持)\n");
					printf("- 11.格式化Vendor分区                    \n");
					printf("- 12.通用格式化Data分区(需要TWRP支持)       \n");
					printf("- 13.格式化Data分区_R11                  \n");
					printf("- 14.格式化Data分区_R11s                \n");
					printf("\n");
					bar2();
					printf("- 0.返回主菜单                                   \n");
					bar1();
					printf("请输入你想要使用的功能序号:");
					if (scanf("%d", &choose2) == 1)
					{
						safe_flush(stdin); //清除stdin
						switch(choose2)
						{
							case 0:
								return -3; //由主函数返回主菜单
							case 8:
								return choose2; //主函数对应功能
							case 9:
								return choose2; //主函数对应功能
							case 10:
								return choose2; //主函数对应功能
							case 11:
								return choose2; //主函数对应功能
							case 12:
								return choose2; //主函数对应功能
							case 13:
								return choose2; //主函数对应功能
							case 14:
								return choose2; //主函数对应功能
							default:
								return -1; //返回值-1，由主函数去除错误输入
						}
					}
					else
						return -1; //返回值-1，由主函数去除错误输入
				case 4:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("               ==128GB主板专区==              \n");
					printf("\n");
					printf("- 15.只扩容System分区                     \n");
					printf("- 16.同时扩容System与Vendor分区           \n");
					printf("- 17.扩容System为5GB、Vendor分区为2GB     \n");
					printf("- 18.还原原分区表                         \n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.返回主菜单                                   \n");
					bar1();
					printf("请输入你想要使用的功能序号:");
					if (scanf("%d", &choose2) == 1)
					{
						safe_flush(stdin); //清除stdin
						switch(choose2)
						{
							case 0:
								return -3; //由主函数返回主菜单
							case 15:
								return choose2; //主函数对应功能
							case 16:
								return choose2; //主函数对应功能
							case 17:
								return choose2; //主函数对应功能
							case 18:
								return choose2; //主函数对应功能
							default:
								return -1; //返回值-1，由主函数去除错误输入
						}
					}
					else
						return -1; //返回值-1，由主函数去除错误输入
				default:
					return -1; //返回值-1，由主函数去除错误输入
			}
        }
        else
        {
			safe_flush(stdin); //清除stdin
            system("CLS"); //清屏
            return -1; //返回值-1，由主函数去除错误输入
        }
    }
}
int enter_system_partation_size(void)
{
	int partation_size;
	while (1)
    {
        printf("请输入您要扩容的System分区大小(单位为MB,范围为3320~10240):"); //限定范围，防止误操作
        if (scanf("%d", &partation_size) == 1)
		{
        	safe_flush(stdin); //清除stdin
        	if (partation_size <= 10240 && partation_size >= 3320)
            	return partation_size;
        	else
        	{
            	printf("您输入的数据过大或过小,出于保护,请您重新输入System分区大小\n");
            	continue;
        	}
		}
		else
		{
			safe_flush(stdin); //清除stdin
			printf("您输入的数据有误,请重新输入\n");
			continue;
		}
    }
}
int enter_vendor_partation_size(void)
{
	int partation_size;
	while (1)
    {
        printf("请输入您要扩容的Vendor分区大小(单位为MB,范围为1024~3072):"); //限定范围，防止误操作
        if (scanf("%d", &partation_size) == 1)
		{
        	safe_flush(stdin); //清除stdin
        	if (partation_size <= 3072 && partation_size >= 1024)
            	return partation_size;
        	else
        	{
            	printf("您输入的数据过大或过小,出于保护,请您重新输入Vendor分区大小\n");
            	continue;
        	}
		}
		else
		{
			safe_flush(stdin); //清除stdin
			printf("您输入的数据有误,请重新输入\n");
			continue;
		}
    }
}
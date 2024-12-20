#include <stdio.h>
#include <stdlib.h>

#define VERSION "2.5"

void system_plus(const char *command); //����system����,���Ǽ��������Ƿ�ִ�гɹ��ж�
void bar1(void); //�ָ���1(=====)
void bar2(void); //�ָ���2(-----)
void confirm_operation(void); //ѯ���û��Ƿ��������
void safe_flush(FILE *fp); //���stdin����(����)
void init(void); //����豸����״̬������parted
void umount_partition(void); //�������System,Vendor,Userdata����
void parted_rm_partition1(void); //ɾ��System,Userdata����
void parted_rm_partition2(void); //ɾ��System,Vendor,Userdata����
void parted_rm_partition3(void); //ɾ��System,System_bak,Vendor,Userdata,Userdata_bak����
void clearscreen(void); //�滻������CLSΪclear���ɼ���Linux/UNIX
void checkfile(char *filename); //����ļ��Ƿ����
void check_platfrom_and_parted_windows(void); //������Windows������������Platfrom-Tool��Parted�Ƿ����
void check_platfrom_and_parted_linux(void); //������Linux������������Platfrom-Tool��Parted�Ƿ����

int menu(void); //�˵�
int enter_system_partition_size(void); //����System������С����
int enter_vendor_partition_size(void); //����Vendor������С����

int main(void) {
	const int system_partition_start = 5604;
	int system_partition_end;
	int system_bak_partition_start;
	int system_bak_partition_end;
	int vendor_partition_start;
	int vendor_partition_end;
	int userdata_partition_start;
	int userdata_partition_end;
	int userdata_bak_partition_start;
	int userdata_bak_partition_end;
	int partition_size;
	char mkpart_command[70];

#ifdef _WIN32
	check_platfrom_and_parted_windows();
#else
	check_platfrom_and_parted_linux();
#endif

	bar1();
	printf("����ϵͳ��ԭ�ȴ��ڵ�adb����...\n");
	system("taskkill /f /im adb.exe"); //����adb����,��ֹ������һ����������
	bar1();
	printf("����adb����...\n");
	system("adb start-server");
	bar1();
	clearscreen();

	bar1();
	printf("                      ע������                  \n");
	bar2();
	printf("1.��ʹ�ñ�����ǰ��ȷ������ TWRP �汾���ڵ��� 3.6.x  \n");
	printf("2.��ȷ�������ֻ�δʹ�ù����ݰ���������              \n");
	bar1();
	printf("�밴�س�������");
	getchar();
	clearscreen();

	while (1) {
		switch (menu()) {
			case -3: //�������˵�
				continue;
			case -2: //�˳�����
				return 0;
			case -1: //ȥ����������
				bar1();
				printf("\n");
				printf("\n");
				printf("          ���������ֵ��������������!        \n");
				printf("\n");
				printf("\n");
				bar1();
				break;
			case 1: //Fastboot������Recovery
				bar1();
				printf("���ڼ���豸����״̬,�����ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("echo boot-recovery > misc.bin"); //����misc�ļ�
				system_plus("fastboot flash misc misc.bin");
				system_plus("del misc.bin"); //�����ļ�
				printf("�豸������!   ״̬:Fastboot\n");
				system_plus("fastboot reboot");
				bar1();
				break;
			case 2: //����״̬������Recovery
				bar1();
				printf("���ڼ���豸����״̬,�����ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-device devices");
				printf("�豸������!   ״̬:System\n");
				system_plus("adb reboot recovery");
				bar1();
				break;
			case 3: //ֻ����System���� 
				userdata_partition_end = 62500;

				init();
				partition_size = enter_system_partition_size();
				system_partition_end = system_partition_start + partition_size;
				userdata_partition_start = system_partition_end;
				confirm_operation();
				umount_partition();
				parted_rm_partition1();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Data����
				break;
			case 4: //ͬʱ����System��Vendor����
				userdata_partition_end = 62500;

				init();
				partition_size = enter_system_partition_size();
				system_partition_end = system_partition_start + partition_size;
				vendor_partition_start = system_partition_end;
				partition_size = enter_vendor_partition_size();
				vendor_partition_end = vendor_partition_start + partition_size;
				userdata_partition_start = vendor_partition_end;
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�Vendor����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partition_start, vendor_partition_end);
				system_plus(mkpart_command); //����Vendor����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Data����
				break;
			case 5: //����SystemΪ5GB��Vendor����Ϊ2GB
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 5605 10725");
				printf("�����µ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 10726 12774");
				printf("�����µ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 12775 62.5g");
				printf("�������\n");
				printf("�������豸...\n");
				break;
			case 6: //��ԭԭ������
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�ָ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
				printf("�ָ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
				printf("�ָ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 62.5g");
				printf("�ָ����\n");
				break;
			case 7: //��ȡ�ֻ���ǰ����������
				init();
				system_plus("adb shell parted /dev/block/mmcblk0 print > PartitionTable.txt");
				printf("�ѳɹ����ֻ���ǰ���������������PartitionTable.txt,�����!\n");
				break;
			case 8: //TWRP�Դ���ʽ��System����(��ҪTWRP֧��)
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount system");
				printf("��ʼ��ʽ��System����\n");
				system_plus("adb shell twrp wipe system");
				printf("��ʽ��System�������\n");
				break;
			case 9: //��ʽ��System����
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount system");
				printf("��ʼ��ʽ��System����\n");
				system_plus("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/system");
				printf("��ʽ��System�������\n");
				break;
			case 10: //TWRP�Դ���ʽ��Vendor����(��ҪTWRP֧��)
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount vendor");
				printf("��ʼ��ʽ��Vendor����\n");
				system_plus("adb shell twrp wipe vendor");
				printf("��ʽ��Vendor�������\n");
				break;
			case 11: //��ʽ��Vendor����
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount vendor");
				printf("��ʼ��ʽ��Vendor����\n");
				system_plus("adb shell mke2fs -t ext2 /dev/block/bootdevice/by-name/vendor");
				printf("��ʽ��Vendor�������\n");
				break;
			case 12: //ͨ�ø�ʽ��Data����(��ҪTWRP֧��)
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount data");
				printf("��ʼ��ʽ��Data����\n");
				system_plus("adb shell twrp format data");
				printf("��ʽ��Data�������\n");
				break;
			case 13: //��ʽ��Data����_R11
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system_plus("adb root");
				bar2();
				printf("�������...\n");
				system_plus("adb shell twrp umount data");
				printf("��ʼ��ʽ��Data����\n");
				system_plus("adb shell mke2fs -t ext4 /dev/block/bootdevice/by-name/userdata");
				printf("��ʽ��Data�������\n");
				break;
			case 14: //��ʽ��Data����_R11s
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				system("adb root");
				bar2();
				printf("�������...\n");
				system("adb shell twrp umount data");
				printf("��ʼ��ʽ��Data����\n");
				system("adb shell make_f2fs /dev/block/bootdevice/by-name/userdata");
				printf("��ʽ��Data�������\n");
				break;
			case 15: //128G ֻ����System����
				userdata_partition_end = 125000;

				init();
				partition_size = enter_system_partition_size();
				system_partition_end = system_partition_start + partition_size;
				userdata_partition_start = system_partition_end;
				confirm_operation();
				umount_partition();
				parted_rm_partition1();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Data����
				break;
			case 16: //128G ͬʱ����System��Vendor����
				userdata_partition_end = 125000;

				init();
				partition_size = enter_system_partition_size();
				system_partition_end = system_partition_start + partition_size;
				vendor_partition_start = system_partition_end;
				partition_size = enter_vendor_partition_size();
				vendor_partition_end = vendor_partition_start + partition_size;
				userdata_partition_start = vendor_partition_end;
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�Vendor����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partition_start, vendor_partition_end);
				system_plus(mkpart_command); //����Vendor����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Data����
				break;
			case 17: //128G ����SystemΪ5GB��Vendor����Ϊ2GB
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 5605 10725");
				printf("�����µ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 10726 12774");
				printf("�����µ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 12775 125g");
				printf("�������\n");
				break;
			case 18: //128G ��ԭԭ������
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition2();
				printf("�ָ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
				printf("�ָ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
				printf("�ָ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 125g");
				printf("�ָ����\n");
				break;
			case 19: //�Ƴ��ȸ���
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				printf("��ʼ����frp����...\n");
				system_plus("adb shell dd if=/dev/zero of=/dev/block/by-name/frp bs=1k count=512");
				printf("��ʼ��ʽ��Data����...\n");
				system_plus("adb shell twrp format data");
				printf("�Ƴ��ȸ������\n");
				break;
			case 20: //�Ƴ���������
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				printf("����Data����...\n");
				system_plus("adb shell twrp mount data");
				printf("�Ƴ����������ļ�...\n");
				system_plus("adb shell rm -f /data/system/locksettings.db");
				printf("�Ƴ������������\n");
				break;
			case 21: //���ݻ���
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				printf("��ʼ����...\n");
				system_plus("adb shell dd if=/dev/block/mmcblk0p3 of=/tmp/efs1.img");
				system_plus("adb shell dd if=/dev/block/mmcblk0p4 of=/tmp/efs2.img");
				system_plus("adb shell dd if=/dev/block/mmcblk0p20 of=/tmp/fsg.img");
				system_plus("adb shell dd if=/dev/block/mmcblk0p51 of=/tmp/fsc.img");
				system_plus("adb pull /tmp/efs1.img efs1.img");
				system_plus("adb pull /tmp/efs2.img efs2.img");
				system_plus("adb pull /tmp/fsg.img fsg.img");
				system_plus("adb pull /tmp/fsc.img fsc.img");
				printf("�������\n");
				break;
			case 22: //�ָ�����
				checkfile("efs1.img");
				checkfile("efs2.img");
				checkfile("fsg.img");
				checkfile("fsc.img");
				bar1();
				printf("���ڼ���豸����״̬�������ʱ�俨�ڴ˴������豸���豸��������\n");
				system_plus("adb wait-for-recovery devices");
				printf("�豸������!   ״̬:Recovery\n");
				printf("��ʼ�ָ�...\n");
				system_plus("adb push efs1.img /tmp/efs1.img");
				system_plus("adb push efs2.img /tmp/efs2.img");
				system_plus("adb push fsg.img /tmp/fsg.img");
				system_plus("adb push fsc.img /tmp/fsc.img");
				system_plus("adb shell dd if=/tmp/efs1.img of=/dev/block/mmcblk0p3");
				system_plus("adb shell dd if=/tmp/efs2.img of=/dev/block/mmcblk0p4");
				system_plus("adb shell dd if=/tmp/fsg.img of=/dev/block/mmcblk0p20");
				system_plus("adb shell dd if=/tmp/fsc.img of=/dev/block/mmcblk0p51");
				printf("�ָ����\n");
				break;
			case 23: //��ʼ��װ˫ϵͳ(64GB����)
				userdata_bak_partition_end = 62500;

				init();
				confirm_operation();
				//�û������Զ������������������ʼλ��
				partition_size = enter_system_partition_size();
				system_bak_partition_start = system_partition_end = system_partition_start + partition_size;
				vendor_partition_start = system_bak_partition_end = system_bak_partition_start + partition_size;
				partition_size = enter_vendor_partition_size();
				userdata_partition_start = vendor_partition_end = vendor_partition_start + partition_size;
				userdata_bak_partition_start = userdata_partition_end = (int)((userdata_bak_partition_end - userdata_partition_start) / 2) + userdata_partition_start;
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�System_bak����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system_bak EXT4 %d %d", system_bak_partition_start, system_bak_partition_end);
				system_plus(mkpart_command); //����System_bak����
				printf("�����µ�Vendor����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partition_start, vendor_partition_end);
				system_plus(mkpart_command); //����Vendor����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Userdata����
				printf("�����µ�Userdata_bak����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata_bak EXT4 %d %d", userdata_bak_partition_start, userdata_bak_partition_end);
				system_plus(mkpart_command); //����Userdata_bak����
				printf("˫ϵͳ�����ɹ�\n");
				break;
			case 24: //��ʼ��װ˫ϵͳ(128GB����)
				userdata_bak_partition_end = 125000;
				
				init();
				confirm_operation();
				//�û������Զ������������������ʼλ��
				partition_size = enter_system_partition_size();
				system_bak_partition_start = system_partition_end = system_partition_start + partition_size;
				vendor_partition_start = system_bak_partition_end = system_bak_partition_start + partition_size;
				partition_size = enter_vendor_partition_size();
				userdata_partition_start = vendor_partition_end = vendor_partition_start + partition_size;
				userdata_bak_partition_start = userdata_partition_end = (int)((userdata_bak_partition_end - userdata_partition_start) / 2) + userdata_partition_start;
				umount_partition();
				parted_rm_partition2();
				printf("�����µ�System����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system EXT4 %d %d", system_partition_start, system_partition_end);
				system_plus(mkpart_command); //����System����
				printf("�����µ�System_bak����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart system_bak EXT4 %d %d", system_bak_partition_start, system_bak_partition_end);
				system_plus(mkpart_command); //����System_bak����
				printf("�����µ�Vendor����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 %d %d", vendor_partition_start, vendor_partition_end);
				system_plus(mkpart_command); //����Vendor����
				printf("�����µ�Userdata����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 %d %d", userdata_partition_start, userdata_partition_end);
				system_plus(mkpart_command); //����Userdata����
				printf("�����µ�Userdata_bak����...\n");
				sprintf(mkpart_command, "adb shell parted /dev/block/mmcblk0 mkpart userdata_bak EXT4 %d %d", userdata_bak_partition_start, userdata_bak_partition_end);
				system_plus(mkpart_command); //����Userdata_bak����
				printf("˫ϵͳ�����ɹ�\n");
				break;
			case 25: //�л���ϵͳһ
				init();
				confirm_operation();
				system_plus("adb shell parted /dev/block/mmcblk0 name 66 system");
				system_plus("adb shell parted /dev/block/mmcblk0 name 67 system_bak");
				system_plus("adb shell parted /dev/block/mmcblk0 name 70 userdata");
				system_plus("adb shell parted /dev/block/mmcblk0 name 71 userdata_bak");
				system_plus("adb reboot");
				printf("�л���ϵͳһ�ɹ�\n");
				break;
			case 26: //�л���ϵͳ��
				init();
				confirm_operation();
				system_plus("adb shell parted /dev/block/mmcblk0 name 67 system");
				system_plus("adb shell parted /dev/block/mmcblk0 name 66 system_bak");
				system_plus("adb shell parted /dev/block/mmcblk0 name 71 userdata");
				system_plus("adb shell parted /dev/block/mmcblk0 name 70 userdata_bak");
				system_plus("adb reboot");
				printf("�л���ϵͳ���ɹ�\n");
				break;
			case 27: //��ԭ������(64GB����)
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition3();
				printf("�ָ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
				printf("�ָ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
				printf("�ָ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 62.5g");
				printf("�ָ����\n");
				break;
			case 28: //��ԭ������(128GB����)
				init();
				confirm_operation();
				umount_partition();
				parted_rm_partition3();
				printf("�ָ�System����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart system EXT4 1040 4521");
				printf("�ָ�Vendor����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart vendor EXT4 4521 5595");
				printf("�ָ�Userdata����...\n");
				system_plus("adb shell parted /dev/block/mmcblk0 mkpart userdata EXT4 5604 125g");
				printf("�ָ����\n");
				break;
		}
		bar1();
		printf("���س����������˵�");
		getchar();
		clearscreen();
	}
	return 0;
}
void system_plus(const char *command) {
	char choose;

	if (system(command) != 0) {
		printf("��⵽�������ִ��ʧ��,�Ƿ�����(y/N)");
		if (scanf("%c", &choose) == 1) {
			safe_flush(stdin);
			if (choose == 'y' || choose == 'Y') {
				for (int i = 1; i <= 3; i++) {
					printf("��������...(��%d��/��3��)\n", i);
					if (system(command) == 0)
						return;
				}
	   			printf("������ִ��ʧ��,�����Լ�������,����ɵĺ�����߲���е�\n");
				printf("�Ƿ��������(y/N)");
				if (scanf("%c", &choose) == 1) {
					safe_flush(stdin);
					if (choose == 'y' || choose == 'Y')
						return;
					else
						exit(0);
	   			} else
					exit(0);
			} else
				exit(0);
		} else
			exit(0);
	} else
		return;
}
void bar1(void) {
	printf("=================================================\n");
}
void bar2(void) {
	printf("-------------------------------------------------\n");
}
void confirm_operation(void) {
	char choose;

	printf("�˲������ܻᵼ������ֻ���ש,ȷ��Ҫ����������?(y/N)");
	scanf("%c", &choose);
	safe_flush(stdin); //���stdin����
	if (choose == 'Y' || choose == 'y') //�ж��û��������Ƿ�Ϸ�
		return;
	else
		exit(0);
}
void safe_flush(FILE *fp) {
	int ch;

	while( (ch = fgetc(fp)) != EOF && ch != '\n' );
}
void init(void) {
	bar1();
	printf("���ڼ���豸����״̬,�����ʱ�俨�ڴ˴������豸���豸��������\n");
	system_plus("adb wait-for-recovery devices");
	printf("�豸������!   ״̬:Recovery\n");
	system_plus("adb push parted /sbin/parted");
	printf("Parted�Ѿ����͹�ȥ��!������һ������\n");
	system_plus("adb root");
	system_plus("adb shell chmod 0755 /sbin/parted");
	printf("�Ѿ���Parted�������ļ�����755Ȩ����!\n");
	bar2();
}
void umount_partition(void) {
	printf("�������...\n");
	system_plus("adb shell twrp umount system");
	system_plus("adb shell twrp umount vendor");
	system_plus("adb shell twrp umount data");
	bar2();
}
void parted_rm_partition1(void) {
	printf("ɾ��System����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 66");
	printf("ɾ��Userdata����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 69");
	bar2();
}
void parted_rm_partition2(void) {
	printf("ɾ��System����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 66");
	printf("ɾ��Vendor����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 67");
	printf("ɾ��Userdata����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 69");
	bar2();
}
void parted_rm_partition3(void) {
	printf("ɾ��System����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 66");
	printf("ɾ��System_bak����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 67");
	printf("ɾ��Vendor����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 69");
	printf("ɾ��Userdata����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 70");
	printf("ɾ��Userdata_bak����...\n");
	system_plus("adb shell parted /dev/block/mmcblk0 rm 71");
	bar2();
}
void clearscreen(void) {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}
void checkfile(char *filename) {
	FILE *fp;

	fp = fopen(filename, "r");

	if (fp != NULL) {
		fclose(fp);
		return;
	} else {
		printf("�ļ�\"%s\"������,�����������Ŀ¼\n", filename);
		printf("���س����˳�����");
		getchar();
		exit(0);
	}
}
void check_platfrom_and_parted_windows() {
	checkfile("parted");
	//Ҫ����ʹ��adb�������������¼����ļ�
	checkfile("adb.exe");
	checkfile("AdbWinApi.dll");
	checkfile("AdbWinUsbApi.dll");
	checkfile("fastboot.exe");
}
void check_platfrom_and_parted_linux() {
	checkfile("parted");
	//Ҫ����ʹ��adb�������������¼����ļ�
	checkfile("adb");
	checkfile("fastboot");
}
int menu(void) {
	int choose1;
	int choose2;

	bar1();
	printf("     ��ӭʹ�� R11 ϵ�� Parted ���ݹ�����      \n");
	printf("   ����������й��� GitHub ������ MPL ��Դ    \n");
	bar1();
	printf("     ��ȷ������ TWRP �汾���ڵ��� 3.6.x       \n");
	bar1();
	printf("\n");
	printf("- 01.����������\n");
	printf("- 02.���ݹ�����\n");
	printf("- 03.���������\n");
	printf("- 04.128GB����ר��\n");
	printf("- 05.ʵ�ù�����\n");
	printf("- 06.˫ϵͳ������\n");
	printf("\n");
	bar2();
	printf("- 0.�˳�                           �汾:%s\n", VERSION);
	bar1();
	 while (1) {
		printf("����������Ҫʹ�õĹ������:");
		if (scanf("%d", &choose1) == 1) {
			safe_flush(stdin); //���stdin

			clearscreen();
			switch (choose1) { //�����˵�
				case 0:
					clearscreen();
					return -2; //����ֵ-2,���������˳�����
				case 1:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==����������==              \n");
					printf("\n");
					printf("- 01.Fastboot������Recovery\n");
					printf("- 02.����״̬������Recovery\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 1:
							case 2:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				case 2:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==���ݹ�����==              \n");
					printf("\n");
					printf("- 03.ֻ����System����\n");
					printf("- 04.ͬʱ����System��Vendor����\n");
					printf("- 05.����SystemΪ5GB��Vendor����Ϊ2GB\n");
					printf("- 06.��ԭԭ������\n");
					printf("- 07.��ȡ�ֻ���ǰ����������\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 3:
							case 4:
							case 5:
							case 6:
							case 7:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				case 3:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==���������==              \n");
					printf("\n");
					printf("- 08.TWRP�Դ���ʽ��System����(��ҪTWRP֧��)\n");
					printf("- 09.��ʽ��System����\n");
					printf("- 10.TWRP�Դ���ʽ��Vendor����(��ҪTWRP֧��)\n");
					printf("- 11.��ʽ��Vendor����\n");
					printf("- 12.ͨ�ø�ʽ��Data����(��ҪTWRP֧��)\n");
					printf("- 13.��ʽ��Data����_R11\n");
					printf("- 14.��ʽ��Data����_R11s\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 8:
							case 9:
							case 10:
							case 11:
							case 12:
							case 13:
							case 14:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				case 4:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("               ==128GB����ר��==              \n");
					printf("\n");
					printf("- 15.ֻ����System����\n");
					printf("- 16.ͬʱ����System��Vendor����\n");
					printf("- 17.����SystemΪ5GB��Vendor����Ϊ2GB\n");
					printf("- 18.��ԭԭ������\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 15:
							case 16:
							case 17:
							case 18:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				case 5:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("                ==ʵ�ù�����==                 \n");
					printf("\n");
					printf("- 19.�Ƴ��ȸ���\n");
					printf("- 20.�Ƴ���������\n");
					printf("- 21.���ݻ���\n");
					printf("- 22.�ָ�����\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					printf("\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 19:
							case 20:
							case 21:
							case 22:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				case 6:
					bar1();
					printf(" <                                               \n");
					bar2();
					printf("               ==˫ϵͳ������==                \n");
					printf("��ǰ����:\n");
					printf(" 1.��ʹ�ô˴�����ǰ��ȷ�����߱���ש������\n");
					printf(" 2.˫ϵͳ������Boot,Vendor��Firmware,�û�����ƽ��\n");
					printf(" 3.����ϵͳֻ��Ϊ����һ��ϵͳ��������\n");
					bar2();
					printf("- 23.��ʼ��װ˫ϵͳ(64GB����)\n");
					printf("- 24.��ʼ��װ˫ϵͳ(128GB����)\n");
					printf("- 25.�л���ϵͳһ\n");
					printf("- 26.�л���ϵͳ��\n");
					printf("- 27.��ԭ������(64GB����)\n");
					printf("- 28.��ԭ������(128GB����)\n");
					bar2();
					printf("- 0.�������˵�\n");
					bar1();
					printf("����������Ҫʹ�õĹ������:");
					if (scanf("%d", &choose2) == 1) {
						safe_flush(stdin); //���stdin

						switch(choose2) {
							case 0:
								clearscreen();
								return -3; //���������������˵�
							case 23:
							case 24:
							case 25:
							case 26:
							case 27:
							case 28:
								clearscreen();
								return choose2; //��������Ӧ����
							default:
								clearscreen();
								return -1; //����ֵ-1����������ȥ����������
						}
					} else
						clearscreen();
						return -1; //����ֵ-1����������ȥ����������
				default:
					clearscreen();
					return -1; //����ֵ-1����������ȥ����������
			}
		} else {
			safe_flush(stdin); //���stdin
			clearscreen();
			return -1; //����ֵ-1����������ȥ����������
		}
	}
}
int enter_system_partition_size(void) {
	int partition_size;
	while (1) {
		printf("��������Ҫ���ݵ�System������С(��λΪMB,��ΧΪ3481~10240):"); //�޶���Χ����ֹ�����
		if (scanf("%d", &partition_size) == 1) {
			safe_flush(stdin); //���stdin
			if (partition_size <= 10240 && partition_size >= 3481)
				return partition_size;
			else {
				printf("����������ݹ�����С,���ڱ���,������������System������С\n");
				continue;
			}
		} else {
			safe_flush(stdin); //���stdin
			printf("���������������,����������\n");
			continue;
		}
	}
}
int enter_vendor_partition_size(void) {
	int partition_size;
	while (1) {
		printf("��������Ҫ���ݵ�Vendor������С(��λΪMB,��ΧΪ1074~3072):"); //�޶���Χ����ֹ�����
		if (scanf("%d", &partition_size) == 1) {
			safe_flush(stdin); //���stdin
			if (partition_size <= 3072 && partition_size >= 1074)
				return partition_size;
			else {
				printf("����������ݹ�����С,���ڱ���,������������Vendor������С\n");
				continue;
			}
		} else {
			safe_flush(stdin); //���stdin
			printf("���������������,����������\n");
			continue;
		}
	}
}

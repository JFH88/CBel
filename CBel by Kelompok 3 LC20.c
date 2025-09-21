/*
	CBel
	Dibuat oleh
	Kelompok 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define RESET "\x1b[0m"

#define MAX_USERS 100

const int STUDYREMINDER = 2; // 2 hari
const int jumlahHurufCase = 1000;
const int jumlahHurufSoal = 500;
const int jumlahHurufHint = 300;
const int jumlahHurufJawaban = 100;
char USERNAME[100];

typedef struct time
{
	int tahun;
	int bulan;
	int tanggal;
	int jam;
	int menit;
	int detik;
}TIME;

SYSTEMTIME str_t;

struct studRemind
{
	char userNama[100];
	TIME userWaktu;
};

struct User {
    char username[100];
    char password[100];
};

// STUDY REMINDER
long long int bulanKeHari(TIME t);
long long int bedaHari(TIME t1, TIME t2); // t1 = study reminder, t2 = dari sistem
int keDetik(TIME t);
int perluStudyReminder(SYSTEMTIME waktuSistem, TIME studyReminder);
void setStudyReminder(char *username); // main study reminder
void notification(char *username);  //main study reminder

// BELAJAR
void tostring(char str[], int num);
void soal(int level);
void studyCase(int level);
void aksesSoal();
void aksesStudyCase();
void pilihKegiatan(); // main belajar
void levelSoal();
void levelStudyCase();
void bacafile(int level);
void aksesMateri();
void levelMateri();

// LOGIN & REGISTER
void clear();
void registerUser(struct User users[], int *userCount, char *username, char *password);
int loginUser(struct User users[], int userCount, char *username, char *password);
void loginAndRegister(); // main login & register

// LAINNYA
void animasi(char kalimat[], int miliDetik);

int main()
{
	loginAndRegister();
	
	return 0;
}

long long int bulanKeHari(TIME t)
{
	long long int hari = 0;
	if(t.bulan == 1)
	{
		hari += t.tanggal;
	}
	else if(t.bulan == 2)
	{
		hari += t.tanggal + 31;
	}
	else if(t.bulan == 3)
	{
		hari += t.tanggal + 61;
	}
	else if(t.bulan == 4)
	{
		hari += t.tanggal + 92;
	}
	else if(t.bulan == 5)
	{
		hari += t.tanggal + 122;
	}
	else if(t.bulan == 6)
	{
		hari += t.tanggal + 153;
	}
	else if(t.bulan == 7)
	{
		hari += t.tanggal + 183;
	}
	else if(t.bulan == 8)
	{
		hari += t.tanggal + 214;
	}
	else if(t.bulan == 9)
	{
		hari += t.tanggal + 245;
	}
	else if(t.bulan == 10)
	{
		hari += t.tanggal + 275;
	}
	else if(t.bulan == 11)
	{
		hari += t.tanggal + 306;
	}
	else if(t.bulan == 12)
	{
		hari += t.tanggal + 336;
	}
	
	if(t.bulan >= 3 && t.tahun % 4 == 0)
	{
		hari -= 1;
	}
	else if(t.bulan >= 3)
	{
		hari -= 2;
	}
	
	return hari;
}

long long int bedaHari(TIME t1, TIME t2) // t1 = study reminder, t2 = dari sistem
{
	long long int hari=0;
	
	if(t2.tahun >= t1.tahun + 2)
	{
		hari += 365;
	}
	else if(t2.tahun == t1.tahun + 1)
	{
		if(t2.bulan >= 2)
		{
			hari += 365;
		}
		else if(t1.bulan <= 11)
		{
			hari += 365;
		}
		else // kalau t2 bulan januari && t1 bulan desember
		{
			if(t2.tanggal > 2 || t1.tanggal < 30)  //kalau tanggalnya t2 lebih besar dari 2 / tanggalnya t1 lebih kecil dari 30
			{
				hari += 365;
			}
			else if(t1.tanggal == 30 && t2.tanggal == 1)
			{
				hari += 2;
			}
			else if(t1.tanggal == 30 && t2.tanggal == 2)
			{
				hari += 3;
			}
			else if(t1.tanggal == 31 && t2.tanggal == 1)
			{
				hari += 1;
			}
			else if(t1.tanggal == 31 && t2.tanggal == 2)
			{
				hari += 2;
			}
		}
	}
	else if(t2.tahun == t1.tahun)
	{
		long long int darit2 = bulanKeHari(t2);
		long long int darit1 = bulanKeHari(t1);
		hari += darit2 - darit1;
	}
	
	return hari;
}

int keDetik(TIME t)
{
	int detik = 0;
	detik = t.jam*3600 + t.menit*60;
	return detik;
}

int perluStudyReminder(SYSTEMTIME waktuSistem, TIME studyReminder)
{
	TIME dariSistem;
	dariSistem.tahun = waktuSistem.wYear;
	dariSistem.bulan = waktuSistem.wMonth;
	dariSistem.tanggal = waktuSistem.wDay;
	dariSistem.jam = waktuSistem.wHour;
	dariSistem.menit = waktuSistem.wMinute;
	dariSistem.detik = waktuSistem.wSecond;
	
	long long int selisihHari = bedaHari(studyReminder, dariSistem);
	
	if(selisihHari > 2)
	{
		return 1;
	}
	else if(selisihHari == 2)
	{
		int sistemDetik = keDetik(dariSistem);
		int studyReminderDetik = keDetik(studyReminder);
		if(sistemDetik > studyReminderDetik)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void setStudyReminder(char *username)
{
	GetSystemTime(&str_t);
	
	TIME waktuStudy;
	
	waktuStudy.jam = str_t.wHour;
	waktuStudy.menit = str_t.wMinute;
	waktuStudy.detik = str_t.wSecond;
	
	if(str_t.wMonth == 1 || str_t.wMonth == 3 || str_t.wMonth == 5 || str_t.wMonth == 7 || str_t.wMonth == 8 || str_t.wMonth == 10 || str_t.wMonth == 12) // bulan dengan 31 hari
	{
		if(str_t.wDay + STUDYREMINDER > 31 && str_t.wMonth == 12)
		{
			waktuStudy.tahun = str_t.wYear+1;
			waktuStudy.bulan = 1;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER - 31;
		}
		else if(str_t.wDay + STUDYREMINDER > 31)
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth + 1;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER - 31;
		}
		else
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER;
		}
	}
	else if(str_t.wMonth == 2 && str_t.wYear%4 == 0) // kalau bulan februari dan tahunnya habis dibagi 4 - 29 hari
	{
		if(str_t.wDay + STUDYREMINDER > 29)
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth + 1;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER - 29;
		}
		else
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER;
		}
	}
	else if(str_t.wMonth == 2) // kalau bulan februari dan tahunnya tidak habis dibagi 4 - 28 hari
	{
		if(str_t.wDay + STUDYREMINDER > 28)
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth + 1;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER - 28;
		}
		else
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER;
		}
	}
	else // bulan dengan 30 hari
	{
		if(str_t.wDay + STUDYREMINDER > 30)
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth + 1;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER - 30;
		}
		else
		{
			waktuStudy.tahun = str_t.wYear;
			waktuStudy.bulan = str_t.wMonth;
			waktuStudy.tanggal = str_t.wDay + STUDYREMINDER;
		}
	}
	
	FILE *readReminder = fopen("reminder.txt", "r");
	
	if(readReminder == NULL)
	{
		printf("gagal membuka reminder.txt\n");
		exit(1);
	}
	
	struct studRemind userStudyReminder[MAX_USERS];
	char c;
	for(c=getc(readReminder);c!='\n';c=getc(readReminder)) {}
	int record = 0, dataRead;
	while(!feof(readReminder))
	{
		dataRead = fscanf(readReminder, "%[^,],%d/%d/%d %d:%d:%d\n", userStudyReminder[record].userNama, &userStudyReminder[record].userWaktu.tanggal, &userStudyReminder[record].userWaktu.bulan, 
		&userStudyReminder[record].userWaktu.tahun, &userStudyReminder[record].userWaktu.jam, &userStudyReminder[record].userWaktu.menit, &userStudyReminder[record].userWaktu.detik);
		
		if(dataRead != 7)
		{
			printf("Ada data yang tidak terbaca. Kemnungkinan ada format yang salah di %d\n", record);
			exit(1);
		}
		else
		{
			record++;
		}
	}
	
	fclose(readReminder);
	
	FILE *writeReminder = fopen("reminder.txt", "w");
	
	if(writeReminder == NULL)
	{
		printf("gagal membuka file reminder.txt\n");
		exit(1);
	}
	
	fprintf(writeReminder, "username,waktu belajar(tanggal/bulan/tahun jam:menit:detik)\n");
	
	int sudahAda = 0;
	int i=0;
	while(i<record)
	{
		if(strcmp(userStudyReminder[i].userNama, username)==0)
		{
			fprintf(writeReminder, "%s,%d/%d/%d %d:%d:%d\n", username, waktuStudy.tanggal, waktuStudy.bulan, waktuStudy.tahun, waktuStudy.jam, waktuStudy.menit, waktuStudy.detik);
			sudahAda = 1;
		}
		else
		{
			fprintf(writeReminder, "%s,%d/%d/%d %d:%d:%d\n", userStudyReminder[i].userNama, userStudyReminder[i].userWaktu.tanggal, userStudyReminder[i].userWaktu.bulan, 
			userStudyReminder[i].userWaktu.tahun, userStudyReminder[i].userWaktu.jam, userStudyReminder[i].userWaktu.menit, userStudyReminder[i].userWaktu.detik);
		}
		i++;
	}
		
	if(sudahAda == 0)  //kalau belum ada, tambahkan data
	{
		fprintf(writeReminder, "%s,%d/%d/%d %d:%d:%d\n", username, waktuStudy.tanggal, waktuStudy.bulan, waktuStudy.tahun, waktuStudy.jam, waktuStudy.menit, waktuStudy.detik);
	}
	
	fclose(writeReminder);
}

void notification(char *username)
{
	FILE *file = fopen("reminder.txt", "r");
	
	if(file == NULL)
	{
		printf("gagal membuka file reminder.txt\n");
		exit(1);
	}
	
	struct studRemind dariFile;
	TIME reminderUser;
	
	char c;
	for(c=getc(file);c!='\n';c=getc(file)) {}
	do{
		fscanf(file, "%[^,],%d/%d/%d %d:%d:%d\n", dariFile.userNama, &dariFile.userWaktu.tanggal, &dariFile.userWaktu.bulan, &dariFile.userWaktu.tahun, 
		&dariFile.userWaktu.jam, &dariFile.userWaktu.menit, &dariFile.userWaktu.detik);
		
		if(strcmp(dariFile.userNama, username)==0)
		{
			reminderUser.tanggal = dariFile.userWaktu.tanggal;
			reminderUser.bulan = dariFile.userWaktu.bulan;
			reminderUser.tahun = dariFile.userWaktu.tahun;
			reminderUser.jam = dariFile.userWaktu.jam;
			reminderUser.menit = dariFile.userWaktu.menit;
			reminderUser.detik = dariFile.userWaktu.detik;
		}
	}while(!feof(file));
	
	GetSystemTime(&str_t);
	
	int perluDiRemind = perluStudyReminder(str_t, reminderUser);
	
	if(perluDiRemind == 1)
	{
		printf(RED "\tJangan lupa untuk belajar. Anda belum belajar setidaknya sudah 2 hari\n\n" RESET);
	}
	else if(perluDiRemind == 0)
	{
		printf(YELLOW "\tSemangat belajar ngoding\n\n" RESET);
	}
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10; // rem = 1
        num = num / 10; // num = 11
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void soal(int level)
{
	char levelStr[3];
	tostring(levelStr, level);
	char namaFile[12] = "soal_";
	strcat(namaFile, levelStr);
	strcat(namaFile, ".txt");
	
	FILE *fileSoal = fopen(namaFile, "r");
	
	if(fileSoal == NULL)
	{
		printf("File %s tidak ditemukan\n", namaFile);
		exit(1);
	}
	
	do{
		system("cls");
		char kalimatSoal[jumlahHurufSoal];
		fscanf(fileSoal, "%[^@]@\n", kalimatSoal);
		
		char jawaban[jumlahHurufJawaban];
		fscanf(fileSoal, "%[^@]@\n", jawaban);
		
		char hint[jumlahHurufHint];
		fscanf(fileSoal, "%[^@]@\n", hint);
		
		int jawabanBenar = 0;
		int jumlahKaliMenjawab = 0;
		do{
			system("cls");
			printf(CYAN "\n\t\t\t\t\t\t\tSOAL %d\n\n", level);
			printf("%s\n", kalimatSoal);
			char jawabanUser[jumlahHurufJawaban];
			printf("Jawab >> ");
			fflush(stdin);
			scanf("%[^\n]", jawabanUser);
			
			int i=0;
			while(jawabanUser[i] != '\0')
			{
				if(jawabanUser[i]>=65 && jawabanUser[i]<=90)
					jawabanUser[i] = tolower(jawabanUser[i]);
				i++;
			}
			
			if(strcmp(jawaban, jawabanUser)==0)
			{
				printf("Jawaban benar\n\n");
				jawabanBenar = 1;
			}
			else
			{
				printf("Jawaban salah\n\n");
				jawabanBenar = 0;
				jumlahKaliMenjawab++;
			}
			
			if(jumlahKaliMenjawab >= 6 && jawabanBenar == 0)
			{
				printf("Kunci : %s\n\n", jawaban);
			}
			else if(jumlahKaliMenjawab >= 3 && jawabanBenar == 0)
			{
				printf("Bantuan : %s\n\n", hint);
			}
			printf("(tekan apapun untuk melanjutkan)\n\n");
			getch();
		}while(jawabanBenar == 0);
	}while(!feof(fileSoal));
	
	fclose(fileSoal);
	
	animasi("Kembali ke halaman utama", 10);
	animasi("...", 500);
}

void studyCase(int level)
{
	char levelStr[3];
	tostring(levelStr, level);
	char namaFile[12] = "case_";
	strcat(namaFile, levelStr);
	strcat(namaFile, ".txt");
	
	FILE *fileSoal = fopen(namaFile, "r");
	
	if(fileSoal == NULL)
	{
		printf("File %s tidak ditemukan\n", namaFile);
		exit(1);
	}
	
	char ceritaCase[jumlahHurufCase];
	fscanf(fileSoal, "%[^@]@\n", ceritaCase);
	
	do{
		system("cls");
		
		char kalimatSoal[jumlahHurufSoal];
		fscanf(fileSoal, "%[^@]@\n", kalimatSoal);
		
		char jawaban[jumlahHurufJawaban];
		fscanf(fileSoal, "%[^@]@\n", jawaban);
		
		char hint[jumlahHurufHint];
		fscanf(fileSoal, "%[^@]@\n", hint);
		
		int jawabanBenar = 0;
		int jumlahKaliMenjawab = 0;
		do{
			system("cls");
			printf(YELLOW "\n\t\t\t\t\t\t\tSTUDY CASE %d\n\n", level);
			printf("%s\n\n", ceritaCase);
			printf("%s\n", kalimatSoal);
			char jawabanUser[jumlahHurufJawaban];
			printf(YELLOW "Jawab >> ");
			fflush(stdin);
			scanf("%[^\n]", jawabanUser);
			
			//jawaban user tidak divalidasi, karena case study kebanyakan adalah soal pemrograman dan bahasa C case sensitive
			
			if(strcmp(jawaban, jawabanUser)==0)
			{
				printf("Jawaban benar\n\n");
				jawabanBenar = 1;
			}
			else
			{
				printf("Jawaban salah\n\n");
				jawabanBenar = 0;
				jumlahKaliMenjawab++;
			}
			
			if(jumlahKaliMenjawab >= 6 && jawabanBenar == 0)
			{
				printf("Kunci : %s\n\n", jawaban);
			}
			else if(jumlahKaliMenjawab >= 3 && jawabanBenar == 0)
			{
				printf("Bantuan : %s\n\n", hint);
			}
			printf("(tekan apapun untuk melanjutkan)\n\n");
			getch();
		}while(jawabanBenar == 0);
	}while(!feof(fileSoal));
	
	fclose(fileSoal);
	
	animasi("Kembali ke halaman utama", 10);
	animasi("...", 500);
}

void aksesSoal()
{
	int pilihLevel = 0;
	do{
		system("cls");
		printf(CYAN "\n\t\t\t\t\t\t\tSOAL\n\n");
		levelSoal();
		fflush(stdin);
		scanf("%d", &pilihLevel);
		printf(RESET);
	}while(pilihLevel<1 || pilihLevel>49);
	
	soal(pilihLevel);
}

void aksesStudyCase()
{
	int pilihLevel = 0;
	do{
		system("cls");
		printf(CYAN "\n\t\t\t\t\t\t\tSTUDY CASE\n\n");
		levelStudyCase();
		fflush(stdin);
		scanf("%d", &pilihLevel);
		printf(RESET);
	}while(pilihLevel<1 || pilihLevel>11);
	
	studyCase(pilihLevel);
}

void pilihKegiatan()
{
	system("cls");
	notification(USERNAME);
	int pilih=0;
	do{
		do{
			if(pilih != 0)
			{
				system("cls");
			}
			printf(CYAN "Pilih kegiatan yang ingin anda lakukan:\n");
			printf("1. Membaca materi\n");
			printf("2. Mengerjakan soal\n");
			printf("3. Mengerjakan study case\n");
			printf("4. logout\n");
			printf(">> ");
			fflush(stdin);
			scanf("%d", &pilih);
			printf(RESET);
		}while(pilih<1 || pilih>4);
		
		switch(pilih)
		{
			case 1:
				aksesMateri();
				break;
			case 2:
				aksesSoal();
				break;
			case 3:
				aksesStudyCase();
				break;
		}
	}while(pilih != 4);
	
	setStudyReminder(USERNAME);
	
	animasi("keluar dari akun", 10);
	animasi("...", 500);
}

void levelSoal()
{
	printf("\n\t\t\tLevel\tTema\n\n");
	printf("\t\t\t1\tPengantar Pemrograman\n");
	printf("\t\t\t2\tPengantar bahasa C\n");
	printf("\t\t\t3\tVariabel\n");
	printf("\t\t\t5\tSymbolic constant & Constanta\n");
	printf("\t\t\t4\tTipe data & format specifier\n");
	printf("\t\t\t6\tOutput dalam C\n");
	printf("\t\t\t7\tInput dalam C\n");
	printf("\t\t\t8\tPengantar operator & operand\n");
	printf("\t\t\t9\tArithmetic operator\n");
	printf("\t\t\t10\tRelational Operator\n");
	printf("\t\t\t11\tBoolean & Logical operator \n");
	printf("\t\t\t12\tPengantar selection\n");
	printf("\t\t\t13\tSelection: If\n");
	printf("\t\t\t14\tSelection: If - Else\n");
	printf("\t\t\t15\tSelection: nested If\n");
	printf("\t\t\t16\tSelection: Switch case\n");
	printf("\t\t\t17\tSelection: Ternary operator \n");
	printf("\t\t\t18\tPengantar repetition\n");
	printf("\t\t\t19\tRepetition: for-loop\n");
	printf("\t\t\t20\tRepetition: while-loop\n");
	printf("\t\t\t21\tRepetition: do while-loop \n");
	printf("\t\t\t22\tRepetition: break & continue\n");
	printf("\t\t\t23\tPengantar pointer\n");
	printf("\t\t\t24\tpointer: deklarasi & penggunaan\n");
	printf("\t\t\t25\tpointer: penjelasan & contoh\n");
	printf("\t\t\t26\tPengantar array\n");
	printf("\t\t\t27\tArray: deklarasi & penggunaan\n");
	printf("\t\t\t28\tArray: Penjelasan & contoh\n");
	printf("\t\t\t29\tArray: two-dimensional & three-dimensional\n");
	printf("\t\t\t30\tArray of character (string)\n");
	printf("\t\t\t31\tString manipulation\n");
	printf("\t\t\t32\tModular programming\n");
	printf("\t\t\t33\tPengantar function: library VS function \n");
	printf("\t\t\t34\tFunction: declaration & prototipe \n");
	printf("\t\t\t35\tFunction: scope of variable \n");
	printf("\t\t\t36\tFunction: Parameter\n");
	printf("\t\t\t37\tFunction: Recursive\n");
	printf("\t\t\t38\tStruct\n");
	printf("\t\t\t39\tNested struct & array of struct\n");
	printf("\t\t\t40\tTypedef\n");
	printf("\t\t\t41\tEnumerasi\n");
	printf("\t\t\t42\tStatic VS dinamic memory allocation\n");
	printf("\t\t\t43\tMalloc\n");
	printf("\t\t\t44\tFree\n");
	printf("\t\t\t45\tPengantar file Processing\n");
	printf("\t\t\t46\tFile Processing: mode\n");
	printf("\t\t\t47\tFile Processing: fopen & fclose\n");
	printf("\t\t\t48\tFile Processing: menulis ke file\n");
	printf("\t\t\t49\tFile Processing: membaca dari file\n\n");
	printf("\t\t\t>> ");
}

void levelStudyCase()
{
	printf("\n\t\t\tLevel\tTipe\n\n");
	printf("\t\t\t1\tTheory Application in Coding\n");
	printf("\t\t\t2\tGuided Coding\n");
	printf("\t\t\t3\tError Analysis\n");
	printf("\t\t\t4\tGuided Coding\n");
	printf("\t\t\t5\tCoding Analysis\n");
	printf("\t\t\t6\tTheory Application in Coding\n");
	printf("\t\t\t7\tError Analysis\n");
	printf("\t\t\t8\tGuided Coding\n");
	printf("\t\t\t9\tCoding Analysis\n");
	printf("\t\t\t10\tError & Coding Analysis\n");
	printf("\t\t\t11\tGuided Coding\n\n");
	printf("\t\t\t>> ");
}

void clear()
{
    system("cls");
}

void registerUser(struct User users[], int *userCount, char *username, char *password)
{
    int i;
    for (i = 0; i < *userCount; ++i) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username sudah ada. Mohon pilih username lain.\n");
            return;
        }
    }

    if (*userCount >= MAX_USERS) {
        printf("Maksimal batas jumlah user tercapai.\n");
        return;
    }

    strcpy(users[*userCount].username, username);
    strcpy(users[*userCount].password, password);
    (*userCount)++;

    FILE *file = fopen("datauser.bin", "ab");
    if (file == NULL) {
        printf("Eror ketika membuka file.\n");
        exit(1);
    }

    fwrite(users + (*userCount - 1), sizeof(struct User), 1, file);
    fclose(file);

    printf("Registrasi berhasil. Silahkan login.\n");
}

int loginUser(struct User users[], int userCount, char *username, char *password)
{
    int i;
    for (i = 0; i < userCount; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login berhasil.\n");
            return 1;
        }
    }
    
    printf("Login gagal. Mohon cek username dan password yang anda input.\n");
    return 0;
}

void loginAndRegister()
{
	struct User users[MAX_USERS];
    int userCount = 0;
    int operasi;
   
    FILE *file = fopen("datauser.bin", "ab+");
    if (file != NULL) {
        while (fread(users + userCount, sizeof(struct User), 1, file) == 1) {
            userCount++;
            if (userCount >= MAX_USERS) {
                break;
            }
        }
        fclose(file);
    } else {
        printf("Eror ketika membuka file.\n");
        exit(1);
    }
    
    char username[100];
    char password[100];

    do {
    	do{
    		clear();
    		fflush(stdin);
	    	printf("Apakah Anda sudah memiliki akun?\n");
	    	printf("1. Login\n");
	    	printf("2. Register\n");
	    	printf("3. Exit\n");
	    	printf(">> ");
	    	fflush(stdin);
	    	scanf("%d", &operasi);
		}while(operasi<1 || operasi>3);
    	
        switch (operasi) {
	        case 1:
	            clear();
	            printf("Login\n");
	            printf("Username: ");
	            fflush(stdin);
	            scanf("%s", username);
	            printf("Password: ");
	            fflush(stdin);
	            scanf("%s", password);
	            int loginValid = loginUser(users, userCount, username, password);
	            if(loginValid == 1)
	            {
	            	animasi("masuk ke program utama", 10);
	            	animasi("...", 500);
	            	strcpy(USERNAME, username);
	            	pilihKegiatan();
				}
				else
				{
					animasi("kembali ke halaman login", 10);
	            	animasi("...", 500);
				}
	            break;
	
	        case 2:
	            clear();
	            printf("Register\n");
	            printf("Username (tanpa spasi): ");
	            fflush(stdin);
	            scanf("%s", username);
	            printf("Password: ");
	            fflush(stdin);
	            scanf("%s", password);
	            registerUser(users, &userCount, username, password);
	            animasi("kembali ke halaman login", 10);
	            animasi("...", 500);
	            setStudyReminder(username);
	            break;
	            
	        case 3:
	            clear();
	            animasi("Terima kasih telah menggunakan program kami", 10);
	            printf("\n");
	            animasi("Keluar dari program", 10);
	            animasi("...", 500);
	            break;
    	}    
    }while (operasi != 3);
}

void bacafile(int level)
{
	char levelStr[3];
	tostring(levelStr, level);
    char filename[17] = "materi";
    strcat(filename, levelStr);
    strcat(filename, ".txt");
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Tidak Ada File\n");
        return;
    }
    
    clear();
    printf(RED "\t\t\t\t\t\t\tMateri ke %d\n\n" RESET, level);

    char line[256];
    
    while (fgets(line, sizeof(line), file) != NULL ) {
        printf(GREEN "%s" RESET, line);
    }
    printf("\n");

    fclose(file);
}

void aksesMateri()
{
	int lanjut;
    do {
    	int level;
        system("cls");
        do{
        	printf(GREEN);
        	printf("\n\t\t\t\t\t\t\tMATERI\n\n");
        	levelMateri();
        	fflush(stdin);
        	scanf("%d", &level);
        	printf(RESET);
		}while(level<1 || level>40);
        bacafile(level);
        do{
        	printf(RED "Masukkan angka 0 untuk berhenti baca materi / 1 untuk lanjut baca materi: ");
        	fflush(stdin);
        	scanf("%d", &lanjut);
        	printf(RESET);
		}while(lanjut<0 || lanjut>1);
    } while (lanjut == 1);
}

void levelMateri()
{
	printf("\n\t\t\tLevel\tTema\n\n");
	printf("\t\t\t1\tPengenalan bahasa pemrograman\n");
	printf("\t\t\t2\tPengenalan bahasa C\n");
	printf("\t\t\t3\tVariabel\n");
	printf("\t\t\t4\tTipe data dalam C\n");
	printf("\t\t\t5\tConstant\n");
	printf("\t\t\t6\tOutput\n");
	printf("\t\t\t7\tInput\n");
	printf("\t\t\t8\tOperator aritmatika\n");
	printf("\t\t\t9\tIncrement/decrement\n");
	printf("\t\t\t10\tOperator relasional\n");
	printf("\t\t\t11\tOperator logika\n");
	printf("\t\t\t12\tTernary Operator\n");
	printf("\t\t\t13\tIf\n");
	printf("\t\t\t14\tIf-else\n");
	printf("\t\t\t15\tIf, else-if, else\n");
	printf("\t\t\t16\tSwitch\n");
	printf("\t\t\t17\tFor loop\n");
	printf("\t\t\t18\tDo-while loop\n");
	printf("\t\t\t19\tWhile\n");
	printf("\t\t\t20\tContinue & break\n");
	printf("\t\t\t21\tPointer\n");
	printf("\t\t\t22\tArray\n");
	printf("\t\t\t23\tMatrix\n");
	printf("\t\t\t24\tArray of character (string)\n");
	printf("\t\t\t25\tString manipulation\n");
	printf("\t\t\t26\tModular programming\n");
	printf("\t\t\t27\tLibrary\n");
	printf("\t\t\t28\tPrototype function\n");
	printf("\t\t\t29\tScope\n");
	printf("\t\t\t30\tParameter\n");
	printf("\t\t\t31\tRekursi & iterasi\n");
	printf("\t\t\t32\tStruct\n");
	printf("\t\t\t33\tNested struct & array of struct\n");
	printf("\t\t\t34\tEnumerasi\n");
	printf("\t\t\t35\tStatic vs dinamic memory allocation\n");
	printf("\t\t\t36\tDinamic memory allocation\n");
	printf("\t\t\t37\tRAM\n");
	printf("\t\t\t38\tPengantar file processing\n");
	printf("\t\t\t39\tPenjelasan dan contoh file processing 1\n");
	printf("\t\t\t40\tPenjelasan dan contoh file processing 2\n\n");
	printf("\t\t\t>> ");
}

void animasi(char kalimat[], int miliDetik)
{
	int i = 0;
	while(kalimat[i] != '\0')
	{
		printf("%c", kalimat[i]);
		Sleep(miliDetik);
		i++;
	}
}

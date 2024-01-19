#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

struct Song {
    char title[100];
    char artist[100];
};

struct Disk {
    char title[100];
    int year;
    struct Song songs[10];
    int numSongs;
};

struct Artist {
    char name[100];
    struct Disk disks[5];
    int numDisks;
    struct Song singles[10];
    int numSongs;
};

void addArtist(struct Artist* artists, int* numArt) {
    FILE* f = fopen("music_data.dat", "a+");
    printf("\n\t������ ��'� ���������: ");
    if (fgets(artists[*numArt].name, sizeof(artists[*numArt].name), stdin) == 0 || artists[*numArt].name[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artists[*numArt].name[strcspn(artists[*numArt].name, "\n")] = '\0';
    (*numArt)++;
    fprintf(f, "\tArtist: %3s\n", artists[*numArt - 1].name);
    printf("\n\t��������� ������ �� ��������.\n");
    fclose(f);
}

void addDiskToArtist(struct Artist* artists, int numArt) {
    FILE* f = fopen("music_data.dat", "a+");
    char artName[100];
    printf("\n\t������ ��'� ���������: ");
    if (fgets(artName, sizeof(artName), stdin) == 0 || artName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artName[strcspn(artName, "\n")] = '\0';
    int artFound = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) != 0) {
        line[strcspn(line, "\n")] = '\0';
        if (strstr(line, artName) != 0 && line[strlen(artName)] == ' ') {
            artFound = 1;
            break;
        }
    }
    if (artFound) {
        system("cls");
        printf("\n\t���������� ���������: %s\n", artName);
        char diskTitle[100];
        printf("\n\t������ ����� �����: ");
        if (fgets(diskTitle, sizeof(diskTitle), stdin) == 0 || diskTitle[0] == '\n') {
            printf("\n\t�������: ����� ����� �� ���� ���� ������.\n");
            fclose(f);
            return;
        }
        diskTitle[strcspn(diskTitle, "\n")] = '\0';
        int releaseYear;
        printf("\n\t������ �� ������� �����: ");
        if (scanf("%d", &releaseYear) != 1) {
            printf("\n\t������� ��� ��������� ���� ������� �����.\n");
            fclose(f);
            return;
        }
        getchar();
        fseek(f, 0, SEEK_END);
        fprintf(f, "\tDisk:    \"%s\", %4d, %7s\n", diskTitle, releaseYear, artName);
        fflush(f);
        artists[numArt].numDisks++;
        printf("\n\t���� ������ �� ��������.\n");
    }
    else printf("\n\t��������� �� ��������.\n");
    fclose(f);
}

void addSongToArtist(struct Artist* artists, int numArt) {
    FILE* f = fopen("music_data.dat", "a+");
    char artName[100];
    printf("\n\t������ ��'� ���������: ");
    if (fgets(artName, sizeof(artName), stdin) == 0 || artName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artName[strcspn(artName, "\n")] = '\0';
    int artistFound = 0;
    char line[256];
    long artistPos = -1;
    while (fgets(line, sizeof(line), f) != 0) {
        line[strcspn(line, "\n")] = '\0';
        if (strstr(line, artName) != 0 && line[strlen(artName)] == ' ') {
            artistFound = 1;
            artistPos = ftell(f) - strlen(line) - 1;
            break;
        }
    }
    if (artistFound) {
        system("cls");
        printf("\n\t���������� ���������: %s\n", artName);
        int addToDisk;
        printf("\n\t������ 1, ��� ������ ���� �� �����, ��� 0, ��� ������ �� ���������� ����: ");
        if (scanf("%d", &addToDisk) != 1) {
            printf("\n\t������� ��� ��������� ������.\n");
            fclose(f);
            return;
        }
        getchar();
        char songTitle[100];
        printf("\n\t������ ����� ���: ");
        if (fgets(songTitle, sizeof(songTitle), stdin) == 0 || songTitle[0] == '\n') {
            printf("\n\t�������: ��'� ��� �� ���� ���� ������.\n");
            fclose(f);
            return;
        }
        songTitle[strcspn(songTitle, "\n")] = '\0';
        int releaseYear;
        if (addToDisk) {
            char diskTitle[100];
            printf("\n\t������ ����� �����: ");
            if (fgets(diskTitle, sizeof(diskTitle), stdin) == 0 || diskTitle[0] == '\n') {
                printf("\n\t�������: ����� ����� �� ���� ���� ������.\n");
                fclose(f);
                return;
            }
            diskTitle[strcspn(diskTitle, "\n")] = '\0';
            int diskFound = 0;
            fseek(f, 0, SEEK_SET);
            while (fgets(line, sizeof(line), f) != 0) {
                line[strcspn(line, "\n")] = '\0';
                if (strstr(line, diskTitle) != 0 && strstr(line, artName) != 0) {
                    diskFound = 1;
                    fseek(f, artistPos, SEEK_SET);
                    int additionalArtists;
                    printf("\n\t������ 1, ���� � �������� ��������� ��� ���� ���, ��� 0, ���� ����: ");
                    if (scanf("%d", &additionalArtists) != 1) {
                        printf("\n\t������� ��� ��������� ������.\n");
                        fclose(f);
                        return;
                    }
                    getchar();
                    if (additionalArtists) {
                        char addArtistsList[256];
                        printf("\n\t������ ����� ���������� ����������, ������� �����: ");
                        if (fgets(addArtistsList, sizeof(addArtistsList), stdin) == 0 || addArtistsList[0] == '\n') {
                            printf("\n\t�������: ����� ���������� �� ������ ���� �������.\n");
                            fclose(f);
                            return;
                        }
                        addArtistsList[strcspn(addArtistsList, "\n")] = '\0';
                        fprintf(f, "\tSong:    \"%s\", %7s, %s, \"%7s\",\n", songTitle, artName, addArtistsList, diskTitle);
                        fflush(f);
                        printf("\n\tϳ��� \"%s\" ������ �� ����� \"%s\" ��������� \"%s\".\n", songTitle, diskTitle, artName);
                    }
                    else {
                        fprintf(f, "\tSong:    \"%s\", %7s, \"%7s\"\n", songTitle, artName, diskTitle);
                        fflush(f);
                        printf("\n\tϳ��� \"%s\" ������ �� ����� \"%s\" ��������� \"%s\".\n", songTitle, diskTitle, artName);
                    }
                    break;
                }
            }
            if (!diskFound)
                printf("\n\t���� \"%s\" ��������� \"%s\" �� ��������.\n", diskTitle, artName);
        }
        else {
            printf("\n\t������ �� ������� ���: ");
            if (scanf("%d", &releaseYear) != 1) {
                printf("\n\t������� ��� ��������� ���� ������� ���.\n");
                fclose(f);
                return;
            }
            getchar();
            int additionalArtists;
            printf("\n\t������ 1, ���� � �������� ��������� ��� ���� ���, ��� 0, ���� ����: ");
            if (scanf("%d", &additionalArtists) != 1) {
                printf("\n\t������� ��� ��������� ������.\n");
                fclose(f);
                return;
            }
            getchar();
            fseek(f, artistPos, SEEK_SET);
            if (additionalArtists) {
                char addArtList[256];
                printf("\n\t������ ����� ���������� ����������, ������� �����: ");
                if (fgets(addArtList, sizeof(addArtList), stdin) == 0 || addArtList[0] == '\n') {
                    printf("\n\t�������: ����� ���������� �� ������ ���� �������.\n");
                    fclose(f);
                    return;
                }
                addArtList[strcspn(addArtList, "\n")] = '\0';
                fprintf(f, "\tSong:    \"%s\", %4d, %7s, %s\n", songTitle, releaseYear, artName, addArtList);
                fflush(f);
                printf("\n\tϳ��� \"%s\" ������ �� ���������� ���� ���������� \"%s\".\n", songTitle, addArtList);
            }
            else {
                fprintf(f, "\tSong:    \"%s\", %4d, %7s\n", songTitle, releaseYear, artName);
                fflush(f);
                printf("\n\tϳ��� \"%s\" ������ �� ���������� ���� ��������� \"%s\".\n", songTitle, artName);
            }
        }
    }
    else printf("\n\t��������� �� ��������.\n");
    fclose(f);
}

void displayArtists() {
    FILE* f = fopen("music_data.dat", "a+");
    char line[256];
    printf("\n\t���������, �� ������� � ��� ���:\n");
    while (fgets(line, sizeof(line), f) != 0) {
        if (strstr(line, "Artist") != 0)
            printf("\t%s", line);
    }
    fclose(f);
}

void displayDisks() {
    FILE* f = fopen("music_data.dat", "a+");
    char line[256];
    printf("\n\t����� �� �� ���������, �� ������� � ��� ���:\n");
    while (fgets(line, sizeof(line), f) != 0) {
        if (strstr(line, "Disk") != 0)
            printf("\t%s", line);
    }
    fclose(f);
}

void displaySongs() {
    FILE* f = fopen("music_data.dat", "a+");
    char line[256];
    printf("\n\tϳ��, �� ������� � ��� ���:\n");
    while (fgets(line, sizeof(line), f) != 0) {
        if (strstr(line, "Song") != 0)
            printf("\t%s", line);
    }
    fclose(f);
}

void removeArtist(struct Artist* artists, int* numArt) {
    FILE* f = fopen("music_data.dat", "r");
    char artistName[100];
    printf("\n\t������ ��'� ��������� ��� ���������: ");
    if (fgets(artistName, sizeof(artistName), stdin) == 0 || artistName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artistName[strcspn(artistName, "\n")] = '\0';
    FILE* newFile = fopen("new_music_data.dat", "w");
    int artistFound = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) != 0) {
        line[strcspn(line, "\n")] = '\0';
        if (strstr(line, artistName) == 0 || line[strlen(artistName)] == ' ')
            fprintf(newFile, "%s\n", line);
        else artistFound = 1;
    }
    fclose(f);
    fclose(newFile);
    if (!artistFound) {
        printf("\n\t��������� \"%s\" �� �������� � ��� �����.\n", artistName);
        remove("new_music_data.dat");
        return;
    }
    remove("music_data.dat");
    rename("new_music_data.dat", "music_data.dat");
    for (int i = 0; i < *numArt; i++) {
        if (strcmp(artists[i].name, artistName) == 0) {
            for (int j = i; j < *numArt - 1; j++)
                artists[j] = artists[j + 1];
            (*numArt)--;
            printf("\n\t��������� \"%s\" �������� � ���� �����.\n", artistName);
            return;
        }
    }
}

void removeDisk(struct Artist* artists, int* numArt) {
    FILE* f = fopen("music_data.dat", "r");
    char artistName[100];
    char diskName[100];
    printf("\n\t������ ��'� ���������: ");
    if (fgets(artistName, sizeof(artistName), stdin) == 0 || artistName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artistName[strcspn(artistName, "\n")] = '\0';
    printf("\n\t������ ����� ����� ��� ���������: ");
    if (fgets(diskName, sizeof(diskName), stdin) == 0 || diskName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    diskName[strcspn(diskName, "\n")] = '\0';
    FILE* newFile = fopen("new_music_data.dat", "w");
    int diskFound = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) != 0) {
        line[strcspn(line, "\n")] = '\0';
        if (strstr(line, artistName) != 0 && strstr(line, diskName) != 0)
            diskFound = 1;
        else fprintf(newFile, "%s\n", line);
    }
    fclose(f);
    fclose(newFile);
    if (!diskFound) {
        printf("\n\t���� \"%s\" ��������� \"%s\" �� �������� � ��� �����.\n", diskName, artistName);
        remove("new_music_data.dat");
        return;
    }
    remove("music_data.dat");
    rename("new_music_data.dat", "music_data.dat");
    for (int i = 0; i < *numArt; i++) {
        if (strcmp(artists[i].name, artistName) == 0) {
            for (int j = i; j < *numArt - 1; j++)
                artists[j] = artists[j + 1];
            (*numArt)--;
            printf("\n\t���� \"%s\" ��������� \"%s\" �������� � ���� �����.\n", diskName, artistName);
            return;
        }
    }
}

void removeSong(struct Artist* artists, int* numArtists) {
    FILE* f = fopen("music_data.dat", "r");
    char artistName[100];
    char diskName[100];
    char songName[100];
    int isIndependent;
    printf("\n\t������ ��'� ���������: ");
    if (fgets(artistName, sizeof(artistName), stdin) == 0 || artistName[0] == '\n') {
        printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    artistName[strcspn(artistName, "\n")] = '\0';
    printf("\n\t������ ����� ���: ");
    if (fgets(songName, sizeof(songName), stdin) == 0 || artistName[0] == '\n') {
        printf("\n\t�������: ����� ��� �� ���� ���� ������.\n");
        fclose(f);
        return;
    }
    songName[strcspn(songName, "\n")] = '\0';
    printf("\n\t�� �� ���������� ����? (1 - ���, 0 - �): ");
    scanf_s("%d", &isIndependent);
    getchar();
    if (!isIndependent) {
        printf("\n\t������ ����� �����: ");
        if (fgets(diskName, sizeof(diskName), stdin) == 0 || diskName[0] == '\n') {
            printf("\n\t�������: ����� ����� �� ���� ���� ������.\n");
            fclose(f);
            return;
        }
        diskName[strcspn(diskName, "\n")] = '\0';
    }
    FILE* newFile = fopen("new_music_data.dat", "w");
    int songFound = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) != 0) {
        line[strcspn(line, "\n")] = '\0';
        if (isIndependent) {
            if (strstr(line, artistName) != 0 && strstr(line, songName) != 0)
                songFound = 1;
            else fprintf(newFile, "%s\n", line);
        }
        else {
            if (strstr(line, artistName) != 0 && strstr(line, diskName) != 0 && strstr(line, songName) != 0)
                songFound = 1;
            else fprintf(newFile, "%s\n", line);
        }
    }
    fclose(f);
    fclose(newFile);
    if (!songFound) {
        if (isIndependent)
            printf("\n\tϳ��� \"%s\" ��������� \"%s\" �� �������� � ��� �����.\n", songName, artistName);
        else printf("\n\tϳ��� \"%s\" �� ����� \"%s\" ��������� \"%s\" �� �������� � ��� �����.\n", songName, diskName, artistName);
        remove("new_music_data.dat");
        return;
    }
    remove("music_data.dat");
    rename("new_music_data.dat", "music_data.dat");
    for (int i = 0; i < *numArtists; i++) {
        if (strcmp(artists[i].name, artistName) == 0) {
            if (isIndependent) {
                for (int k = 0; k < artists[i].numSongs; k++) {
                    if (strcmp(artists[i].singles[k].title, songName) == 0) {
                        for (int l = k; l < artists[i].numSongs - 1; l++)
                            artists[i].singles[l] = artists[i].singles[l + 1];
                        artists[i].numSongs--;
                        printf("\n\tϳ��� \"%s\" ��������� \"%s\" �������� � ���� �����.\n", songName, artistName);
                        return;
                    }
                }
            }
            else {
                for (int j = 0; j < artists[i].numDisks; j++) {
                    if (strcmp(artists[i].disks[j].title, diskName) == 0) {
                        for (int k = 0; k < artists[i].numSongs; k++) {
                            if (strcmp(artists[i].singles[k].title, songName) == 0) {
                                for (int l = k; l < artists[i].numSongs - 1; l++)
                                    artists[i].singles[l] = artists[i].singles[l + 1];
                                artists[i].numSongs--;
                                printf("\n\tϳ��� \"%s\" �� ����� \"%s\" ��������� \"%s\" �������� � ���� �����.\n", songName, diskName, artistName);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void searchByArtist() {
    FILE* f = fopen("music_data.dat", "r");
    char searchName[100];
    int choice;
    char line[256];
    int found = 0;
    do {
        printf("\n\t1 - ������ / ������ �������� ������\n");
        printf("\t0 - ����������� �� ��������� ����\n");
        printf("\n\t������ ����: ");
        if (scanf("%d", &choice) != 1) {
            printf("\n\t������� ��� ��������� ������.\n");
            fclose(f);
            return;
        }
        getchar();
        switch (choice) {
        case 1:
            system("cls");
            printf("\n\t������ ��'� ��������� ��� ������ ������: ");
            if (fgets(searchName, sizeof(searchName), stdin) == 0 || searchName[0] == '\n') {
                printf("\n\t�������: ��'� ��������� �� ���� ���� ������.\n");
                fclose(f);
                return;
            }
            searchName[strcspn(searchName, "\n")] = '\0';
            printf("\n\t������� ������ ��� ��������� \"%s\":\n", searchName);
            while (fgets(line, sizeof(line), f) != 0) {
                line[strcspn(line, "\n")] = '\0';
                if (strstr(line, searchName) != 0) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    printf("\t%s\n", line);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    found = 1;
                }
                else printf("\t%s\n", line);
            }
            break;
        case 0:
            system("cls");
            break;
        default:
            printf("\n\t������� ����. ��������� �����.\n");
        }
    } while (choice != 0);
    if (!found)
        printf("\n\t������ ��� ��������� \"%s\" �� ��������.\n", searchName);
    fclose(f);
}

void menu(struct Artist* artists, int* numArt) {
    int choice;
    do {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
        printf("\n\t\t����:\n\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
        printf("\t1 - ������ ���������, ���� ��� ����\n");
        printf("\t2 - ������� ���������� ��� ����������, ��� ����� ��� ���\n");
        printf("\t3 - �������� ���������, ���� ��� ����\n");
        printf("\t4 - ����� ������� ������\n");
        printf("\t0 - �����\n\n");
        printf("\t��� ����: ");
        scanf_s("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            int choice1;
            do {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                printf("\n\t1 - ������ ���������\n");
                printf("\t2 - ������ ����� ����� �� ���������\n");
                printf("\t3 - ������ ���� ��������� ���� � � ���\n");
                printf("\t0 - ����������� �����\n");
                printf("\n\t��� ����: ");
                scanf_s("%d", &choice1);
                getchar();
                system("cls");
                switch (choice1) {
                case 1:
                    addArtist(artists, numArt);
                    break;
                case 2:
                    addDiskToArtist(artists, *numArt);
                    break;
                case 3:
                    addSongToArtist(artists, *numArt);
                    break;
                case 0:
                    break;
                default:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    printf("\n\n\n\t������� ����. ��������� �� ���.\n\n");
                    break;
                }
            } while (choice1 != 0);
            break;
        case 2:
            int choice2;
            do {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                printf("\n\t1 - ������� ���������� ��� ����������, �� � � ���\n");
                printf("\t2 - ������� ����� �� ����������\n");
                printf("\t3 - ������� ����� ��� ����������\n");
                printf("\t0 - ����������� �����\n");
                printf("\n\t��� ����: ");
                scanf_s("%d", &choice2);
                getchar();
                system("cls");
                switch (choice2) {
                case 1:
                    displayArtists();
                    break;
                case 2:
                    displayDisks();
                    break;
                case 3:
                    displaySongs();
                    break;
                case 0:
                    break;
                default:
                    printf("\n\n\n\t������� ����. ��������� �� ���.\n\n");
                    break;
                }
            } while (choice2 != 0);
            break;
        case 3:
            int choice3;
            do {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                printf("\n\t1 - �������� ��������� \n\t(��� �������� ���������, ����������� ����� � ��� � ����)\n");
                printf("\t2 - �������� ���� ���������\n\t(��� ��������� �����, ����������� �� ��� � �����)\n");
                printf("\t3 - �������� ���� ���������\n");
                printf("\t0 - ����������� �����\n");
                printf("\n\t��� ����: ");
                scanf_s("%d", &choice3);
                getchar();
                system("cls");
                switch (choice3) {
                case 1:
                    removeArtist(artists, numArt);
                    break;
                case 2:
                    removeDisk(artists, numArt);
                    break;
                case 3:
                    removeSong(artists, numArt);
                    break;
                case 0:
                    break;
                default:
                    printf("\n\n\n\t������� ����. ��������� �� ���.\n\n");
                    break;
                }
            } while (choice3 != 0);
            break;
        case 4:
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            searchByArtist();
            break;
        case 0:
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            printf("\n\n\n\t�����, �� �� ����������� \"������� ��������\"!\n\n\t�� ���������!\n\n\n");
            return;
        default:
            system("cls");
            printf("\n\n\n\t������� ����. ��������� �� ���.\n\n");
            break;
        }
    } while (choice != 0);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    struct Artist artists[50];
    int numArtists = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    printf("\n\n\t³��� � \"��������� ��������\"! \n");
    menu(artists, &numArtists);
    return 0;
}

//����� ��������� �������� � ��������� ������� ��������� ��� ���� ���������
//��� �������� ��� �� �������(?) �� �������� ����������� ��� ���������
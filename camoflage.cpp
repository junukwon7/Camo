#include <bits/stdc++.h>
#define WIDTH 1024
#define HEIGHT 768
#define HEADER 54
using namespace std;
struct VAL
{
    int cnt, rr, gg, bb;
};
bool cmp(VAL a, VAL b)
{
    return a.cnt > b.cnt;
}
VAL CNT[5000];
float COLOR[16];
int ICOLOR[16], RI[HEIGHT][WIDTH],GI[HEIGHT][WIDTH],BI[HEIGHT][WIDTH], CODE[HEIGHT][WIDTH];
unsigned char RII[HEIGHT][WIDTH],GII[HEIGHT][WIDTH],BII[HEIGHT][WIDTH],R[HEIGHT][WIDTH],G[HEIGHT][WIDTH],B[HEIGHT][WIDTH];
int main( )
{
    printf("Input file name is \"\\in\\in(ID).bmp ID\".\nID is:");
    int id;
    scanf("%d", &id);
    char title[120];
    sprintf(title, "in\\in%d.bmp", id);
    FILE *fp1 = fopen(title,"rb");
    sprintf(title, "out\\camo_div16_%d.bmp", id);
    FILE *fp2 = fopen(title,"wb");

    int i, j;
    for(i=0; i<HEADER; i++)
        putc(getc(fp1), fp2);

    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            BII[i][j]=getc(fp1);
            BI[i][j] = (int)BII[i][j];
            GII[i][j]=getc(fp1);
            GI[i][j] = (int)GII[i][j];
            RII[i][j]=getc(fp1);
            RI[i][j] = (int)RII[i][j];
            CNT[(BI[i][j]/16) * (1<<8) + (GI[i][j]/16) * (1<<4) + (RI[i][j]/16)].cnt++;
            CNT[(BI[i][j]/16) * (1<<8) + (GI[i][j]/16) * (1<<4) + (RI[i][j]/16)].bb = BI[i][j]/16;
            CNT[(BI[i][j]/16) * (1<<8) + (GI[i][j]/16) * (1<<4) + (RI[i][j]/16)].gg = GI[i][j]/16;
            CNT[(BI[i][j]/16) * (1<<8) + (GI[i][j]/16) * (1<<4) + (RI[i][j]/16)].rr = RI[i][j]/16;
            if(BI[i][j]/16*4 + GI[i][j]/16*4 + RI[i][j]/16*4 <= 40 || BI[i][j]/16*4 + GI[i][j]/16*4 + RI[i][j]/16*4 >= 170)
                CNT[(BI[i][j]/16) * (1<<8) + (GI[i][j]/16) * (1<<4) + (RI[i][j]/16)].cnt = 0;
        }
    }

    sort(CNT, CNT + 4096, cmp);
    float colorsum = 0;

    for(int i = 0; i < 8; i++)
    {
        COLOR[i] = (float)CNT[i].cnt/(float)(HEIGHT * WIDTH);
        colorsum += COLOR[i];
        printf("%d %d %d\n", CNT[i].bb, CNT[i].gg, CNT[i].rr);
    }
    for(int i = 0; i < 8; i++)
    {
        COLOR[i] *= ((float)10000 / colorsum);
        printf("%f\n", COLOR[i]);
    }
    mt19937 engine((unsigned int)time(NULL));                    // MT19937 난수 엔진
    uniform_int_distribution<int> distribution(0, 10000);       // 생성 범위
    auto generator = bind(distribution, engine);

    for(i=0; i<HEIGHT; i+=16)
    {
        for(j=0; j<WIDTH; j+=16)
        {
            int gen = generator(), l, l1 = 0, l2 = 0;
            gen %= 10000;
            if(i > 0 && j > 0)
            {
                l1 = CODE[i - 1][j];
                l2 = CODE[i][j - 1];
            }
            for(l = 1; l <= 8; l++)
            {
                if(l != l1 && l != l2)
                {
                    COLOR[l] -= 900;
                }
                else
                {
                    COLOR[l] += 2700;
                }
            }
            if(l1 == l2) COLOR[l1] += 900;
            for(l = 1; l <= 8; l++)
            {
                ICOLOR[l] = (int)COLOR[l - 1] + ICOLOR[l - 1];
            }
            for(l = 1; l <= 8; l++)
            {
                if(l != l1 && l != l2)
                {
                    COLOR[l] += 900;
                }
                else
                {
                    COLOR[l] -= 2700;
                }
            }
            if(l1 == l2) COLOR[l1] -= 900;
            for(l = 1; l <= 8; l++)
            {
                if(gen >= ICOLOR[l - 1] && gen < ICOLOR[l])
                {
                    break;
                }
                if(l == 8)
                    break;
            }
            CODE[i][j] = l;
            for(int x = 0; x < 16; x++)
            {
                for(int y = 0; y < 16; y++)
                {
                    B[i+x][j+y] = CNT[l - 1].bb * 16;
                    G[i+x][j+y] = CNT[l - 1].gg * 16;
                    R[i+x][j+y] = CNT[l - 1].rr * 16;
                }
            }
        }
    }
    for(i=0; i<HEIGHT; i++)
        for(j=0; j<WIDTH; j++)
        {
            putc(B[i][j],fp2);
            putc(G[i][j],fp2);
            putc(R[i][j],fp2);
        }
    fclose(fp1);
    fclose(fp2);
    return 0;
}

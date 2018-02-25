#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

struct S_POSITION
{
	int nX;
	int nY;
};

struct S_STATE
{
	int nState;
};

S_STATE g_arsTomato[1000][1000]{};
int g_nRow(0), g_nCol(0);

int g_nRipenNumber(0);
int g_nNoRipenNumber(0);
int g_nEmptyNumber(0);

int g_nNewRipenNumber(0);
int g_nCurrentNoRipenNumber(0);
int g_nPreNoRipenNumber(0);

S_POSITION g_arQueue[1000000]{};
int g_nHead(0);
int g_nTail(0);
int g_nQSize(0);

int g_arDP[1000][1000]{};

bool isInsideBox(int nX, int nY);
bool isChangedRipeState();
int addBoxData(int nX, int nY, int nData);
void checkInputNumber(int nData, int nX, int nY);

int enqueue(int nX, int nY);
int dequeue(int *pResX, int *pResY);
int isQueueEmpty();

int spreadRipen(int nX, int nY);
int spreadTarget(int nX, int nY);
int enqueueRipen(int nX, int nY, int nData);

int addDP(int nX, int nY);
bool isDPChecked(int nX, int nY);

int main()
{
	int nData;

	int nCurX;
	int nCurY;

	int nDay;
	///////////////////////
	freopen("input.txt", "r", stdin);
	nData = 0;
	scanf("%d %d", &g_nCol, &g_nRow);
	for (int i = 0; i < g_nRow; ++i)
	{
		for (int j = 0; j < g_nCol; ++j)
		{
			scanf("%d", &nData);
			addBoxData(i, j, nData);
			checkInputNumber(nData, i, j);
			enqueueRipen(i, j, nData);
		}
	}
	//////////////////////
	g_nCurrentNoRipenNumber = g_nNoRipenNumber; nDay = 0;
	nCurX = 0; nCurY = 0;
	g_nPreNoRipenNumber = g_nNoRipenNumber;

	while (g_nCurrentNoRipenNumber != 0)
	{
		//------------
		for (int i = 0; i < g_nRow; ++i)
		{
			for (int j = 0; j < g_nCol; ++j)
			{
				printf("%3d ", g_arsTomato[i][j].nState);
			}
			printf("\n");
		}
		printf("------------------\n");
		//------------
		g_nNewRipenNumber = 0;
		while (g_nRipenNumber != 0)
		{
			dequeue(&nCurX, &nCurY);
			spreadRipen(nCurX, nCurY);
			--g_nRipenNumber;
		}
		g_nRipenNumber = g_nNewRipenNumber;


		if (isChangedRipeState() == false)
		{
			nDay = -1;
			break;
		}

		g_nPreNoRipenNumber = g_nCurrentNoRipenNumber;
		++nDay;
	}
	//////////////////////
	printf("%d\n", nDay);

	return 0;
}

bool isInsideBox(int nX, int nY)
{
	if (nX >= 0 && nX < g_nRow && nY >= 0 && nY < g_nCol)
		return true;
	else
		return false;
}

bool isChangedRipeState()
{
	if (g_nPreNoRipenNumber == g_nCurrentNoRipenNumber)
		return false;
	else
		return true;
}



int addBoxData(int nX, int nY, int nData)
{
	if (isInsideBox(nX, nY) == true)
	{
		g_arsTomato[nX][nY].nState = nData;
		return 1;
	}
	else
		return 0;
}

void checkInputNumber(int nData, int nX, int nY)
{
	if (nData == 0)
		g_nNoRipenNumber++;
	else if (nData == 1)
	{
		addDP(nX, nY);
		g_nRipenNumber++;
	}
	else if (nData == -1)
	{
		addDP(nX, nY);
		g_nEmptyNumber++;
	}
	else
		printf("input error\n");
}

int enqueue(int nX, int nY)
{
	if (isQueueEmpty())
	{
		g_arQueue[g_nTail].nX = nX;
		g_arQueue[g_nTail].nY = nY;
		g_nQSize++;
		return 1;
	}
	else if (!isQueueEmpty())
	{
		g_nTail++;
		g_arQueue[g_nTail].nX = nX;
		g_arQueue[g_nTail].nY = nY;
		g_nQSize++;
		return 1;
	}
	else

	return 0;
}

int dequeue(int *pResX, int *pResY)
{
	if (isQueueEmpty())
	{
		return 0;
	}
	else if (g_nQSize == 1)
	{
		*pResX = g_arQueue[g_nHead].nX;
		*pResY = g_arQueue[g_nHead].nY;
		//g_arQueue[g_nHead].nX = 0;
		//g_arQueue[g_nHead].nY = 0;
		g_nQSize--;
	}
	else
	{
		*pResX = g_arQueue[g_nHead].nX;
		*pResY = g_arQueue[g_nHead].nY;
		//g_arQueue[g_nHead].nX = 0;
		//g_arQueue[g_nHead].nY = 0;
		g_nHead++;
		g_nQSize--;
	}
	return 1;
}

int isQueueEmpty()
{
	if (g_nQSize == 0)
		return 1;
	else
		return 0;
}


int spreadRipen(int nX, int nY)
{
	int nCurX(0);
	int nCurY(0);

	nCurX = nX + 1;
	nCurY = nY;
	spreadTarget(nCurX, nCurY);

	nCurX = nX - 1;
	nCurY = nY;
	spreadTarget(nCurX, nCurY);

	nCurX = nX;
	nCurY = nY + 1;
	spreadTarget(nCurX, nCurY);

	nCurX = nX;
	nCurY = nY - 1;
	spreadTarget(nCurX, nCurY);
	return 0;
}

int spreadTarget(int nX, int nY)
{
	if (isInsideBox(nX, nY) == true && isDPChecked(nX, nY) == false)
	{
		g_arsTomato[nX][nY].nState = 1;
		addDP(nX, nY);

		enqueue(nX, nY);
		++g_nNewRipenNumber;
		--g_nCurrentNoRipenNumber;
		return 1;
	}
	else
		return 0;
}

int enqueueRipen(int nX, int nY, int nData)
{
	if (nData == 1)
	{
		enqueue(nX, nY);
		return 1;
	}
	else
	return 0;
}

int addDP(int nX, int nY)
{
	g_arDP[nX][nY] = 1;
	return 0;
}

bool isDPChecked(int nX, int nY)
{
	if (g_arDP[nX][nY] == 1)
		return true;
	else
		return false;
}

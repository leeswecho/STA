
#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//09.18.2003

void cD3D_Display::Init(VOID* pCA, HWND hMainWnd)
{
//	CoCreateInstance(CLSID_Direct3D9, NULL, CLSCTX_INPROC_SERVER, IID_IDirect3D9, (void**)&pD3D9);
if (DEBUG + DEBUG_D3D >= 1)
	Init_LogFile();

//debugging
HRESULT hr0;
const char* KText;
char NText[50];

	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

//DEBUG	
if (pD3D9 == NULL)
{
	Write_Log("Failure at Direct3DCreate9");
	WriteLog_Enter();

}	else
	{
		Write_Log("Direct3DCreate9 Probably did not fail");
		WriteLog_Enter();
	}

//get current display mode
	hr0 = pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &CurrDisplayMode);

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("GetAdapterDisplayMode: ");
Write_Log((char*)KText);
WriteLog_Enter();
itoa(CurrDisplayMode.Width, NText, 10);
Write_Log("Width: ");
Write_Log(NText);
Write_Log(" Height: ");
itoa(CurrDisplayMode.Height, NText, 10);
Write_Log(NText);
WriteLog_Enter();

//attempt to create a device
//allocate and set presentation parameters
pPP = new D3DPRESENT_PARAMETERS;

	Format = D3DFMT_R5G6B5;

	ZeroMemory((void*)pPP, sizeof(D3DPRESENT_PARAMETERS));

   //Whether we're full-screen or windowed these are the same.
   pPP->BackBufferCount= 1;  //We only need a single back buffer
   pPP->MultiSampleType=D3DMULTISAMPLE_NONE; //No multi-sampling
   pPP->MultiSampleQuality=0; //No multi-sampling
   pPP->SwapEffect     = D3DSWAPEFFECT_DISCARD; // Throw away previous frames, we don't need them
   pPP->hDeviceWindow  = hMainWnd;  //This is our main (and only) window
   pPP->Flags=0;  //No flags to set
   pPP->FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
   pPP->PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT; //Default Presentation rate
   pPP->BackBufferFormat  = Format; //Display format
   pPP->Windowed          = FALSE;
   pPP->BackBufferWidth   = CurrDisplayMode.Width;
   pPP->BackBufferHeight  = CurrDisplayMode.Height;

   hr0 = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, 
						D3DDEVTYPE_HAL, 
						hMainWnd, 
						D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
						pPP,
						&pDevice);

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Create Device: ");
Write_Log((char*)KText);
WriteLog_Enter();



	//now attempt to create a vertex buffer
	hr0 = pDevice->CreateVertexBuffer(sizeof(d3dvertex2) * 4, D3DUSAGE_WRITEONLY, D3D9T_CUSTOMVERTEX, D3DPOOL_MANAGED, &pVertexBuffer, NULL); 

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Create Vertex Buffer: ");
Write_Log((char*)KText);
WriteLog_Enter();

/*
	//TEMP just some temp data to put to screen
	VertexData = new d3dvertex2[4];

	VertexData[0].x = 50; VertexData[0].y = 50; VertexData[0].z = 0; VertexData[0].color = pCA2->pColorTable->Green; VertexData[0].rhw = 1.0f;
	VertexData[1].x = 100; VertexData[1].y = 50; VertexData[1].z = 0; VertexData[1].color = pCA2->pColorTable->Red; VertexData[1].rhw = 1.0f;
	VertexData[2].x = 50; VertexData[2].y = 100; VertexData[2].z = 0; VertexData[2].color = pCA2->pColorTable->WinBlue1; VertexData[2].rhw = 1.0f;
	VertexData[3].x = 100; VertexData[3].y = 100; VertexData[3].z = 0; VertexData[3].color = pCA2->pColorTable->White; VertexData[3].rhw = 1.0f;

//now lock the vertex buffer and memcpy
	hr0 = pVertexBuffer->Lock( 0, 0, (void**)&pBuffer, 0);

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Lock Vertex Buffer: ");
Write_Log((char*)KText);
WriteLog_Enter();

	//now do memcpy
	memcpy(pBuffer, VertexData, sizeof(d3dvertex) * 4);

//unlock
	hr0 = pVertexBuffer->Unlock();

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Unlock Vertex Buffer: ");
Write_Log((char*)KText);
WriteLog_Enter();

//now attempt to render
	hr0 = pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(100,100,100), 1.0f, 0 );

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Device Clear Buffer: ");
Write_Log((char*)KText);
WriteLog_Enter();

	hr0 = pDevice->BeginScene();

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Begin Scene: ");
Write_Log((char*)KText);
WriteLog_Enter();



	hr0 = pDevice->SetFVF(D3D9T_CUSTOMVERTEX);

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Set Vertex Shader: ");
Write_Log((char*)KText);
WriteLog_Enter();

	hr0 = pDevice->SetStreamSource( 0, pVertexBuffer, 0, sizeof(d3dvertex2) );

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Set Stream Source: ");
Write_Log((char*)KText);
WriteLog_Enter();

	//now render!
	hr0 = pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Draw Primitive: ");
Write_Log((char*)KText);
WriteLog_Enter();

	hr0 = pDevice->EndScene();

//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("End Scene: ");
Write_Log((char*)KText);
WriteLog_Enter();

	//now present
	hr0 = pDevice->Present(NULL, NULL, NULL, NULL);
	
//DEBUG
KText = DXGetErrorString9(hr0);
Write_Log("Present: ");
Write_Log((char*)KText);
WriteLog_Enter();
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cD3D_Display::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("Log_cD3D_Display.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cD3D_Display::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_D3D >= 1)
{
DWORD numbyteswritten = 0;

WriteFile( hLogFile,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);

}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cD3D_Display::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cD3D_Display::WriteLog_Enter()
{

if (DEBUG + DEBUG_D3D >= 1)
{
DWORD numbyteswritten = 0;

//write in an automatic ENTER line in the file
char etr[3];

etr[0] = 13;
etr[1] = 10;

WriteFile( hLogFile,
		   etr,
		   2,
		   &numbyteswritten,
		   NULL);
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////


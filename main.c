#include <raylib.h>
#define NONE_INDEX 0xFF
#define MAX_ARR_SIZE 128
#include <stdint.h>
#define RAYGUI_IMPLEMENTATION
#include "extras/raygui.h"   

#define CURB 0x01
#define NOISE 0x02
#define AUTOPILOT 0x04
#define Rectangle WinRectangle
#define CloseWindow WCloseWindow
#define ShowCursor WShowCursor
#define DrawText WDrawText
#include <windows.h> 
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawText
#include <tchar.h>
#include <stdio.h> 

#define BUFSIZE 4096 
 
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;
 
void CreateChildProcess(void); 
void WriteToPipe(void); 
void ReadFromPipe(void); 
void ErrorExit(PTSTR); 
struct var_arr{
    int16_t x[MAX_ARR_SIZE];
    int16_t y[MAX_ARR_SIZE];
    uint16_t heading[MAX_ARR_SIZE];
    uint8_t state[MAX_ARR_SIZE];
    int8_t speed[MAX_ARR_SIZE];
    uint8_t size;
};

void init_arr(struct var_arr* v){
    v->size = 0;
}

void add_to_arr(struct var_arr* v, uint16_t x_pos, uint16_t y_pos, uint16_t heading, uint8_t state){
    v->x[v->size] = x_pos;
    v->y[v->size] = y_pos;
    v->heading[v->size] = heading;
    v->state[v->size] = state;
    v->speed[v->size] = 0;
    v->size++;
}

uint8_t is_inside(Vector2 v, Vector2 v_old){
    int x_min = (((int)v_old.x)/5)*5;
    int x_max = x_min+1;
    int y_min = (((int)v_old.y)/5)*5;
    int y_max = y_min+1;
    return v.x >= x_min && v.x <= x_max && v.y >= y_min && v.y <= y_max;
}
void interaction(){
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
   saAttr.bInheritHandle = TRUE; 
   saAttr.lpSecurityDescriptor = NULL; 

// Create a pipe for the child process's STDOUT. 
 
   if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
      ErrorExit(TEXT("StdoutRd CreatePipe")); 

// Ensure the read handle to the pipe for STDOUT is not inherited.

   if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
      ErrorExit(TEXT("Stdout SetHandleInformation")); 

// Create a pipe for the child process's STDIN. 
 
   if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
      ErrorExit(TEXT("Stdin CreatePipe")); 

// Ensure the write handle to the pipe for STDIN is not inherited. 
 
   if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
      ErrorExit(TEXT("Stdin SetHandleInformation")); 
 
// Create the child process. 
   
   CreateChildProcess();
    WriteToPipe();
}
void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{ 
   TCHAR szCmdline[]=TEXT("powershell");
   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo;
   BOOL bSuccess = FALSE; 
 
// Set up members of the PROCESS_INFORMATION structure. 
 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure. 
// This structure specifies the STDIN and STDOUT handles for redirection.
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process. 
    
   bSuccess = CreateProcess(NULL, 
      szCmdline,     // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 
   
   // If an error occurs, exit the application. 
   if ( ! bSuccess ) 
      ErrorExit(TEXT("CreateProcess"));
   else 
   {
      // Close handles to the child process and its primary thread.
      // Some applications might keep these handles to monitor the status
      // of the child process, for example. 

      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
      
      // Close handles to the stdin and stdout pipes no longer needed by the child process.
      // If they are not explicitly closed, there is no way to recognize that the child process has ended.
      
      CloseHandle(g_hChildStd_OUT_Wr);
      CloseHandle(g_hChildStd_IN_Rd);
   }
}
 
void WriteToPipe(void) 

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{ 
   DWORD dwRead, dwWritten; 
   CHAR chBuf[BUFSIZE];
   BOOL bSuccess = FALSE;
   //printf("HEY\n");
  // memcpy(chBuf,"cd C:\\Users\\berks\\Scenic\n",sizeof("cd C:\\Users\\berks\\Scenic\n"));
  // WriteFile(g_hChildStd_IN_Wr, chBuf, sizeof("cd C:\\Users\\berks\\Scenic\n"), &dwWritten, NULL);
   //FlushFileBuffers(g_hChildStd_IN_Wr);
   //printf("HEY\n");
   //fflush(stdout);
   //memcpy(chBuf,"poetry shell\n",14);
   //WriteFile(g_hChildStd_IN_Wr, chBuf, 14, &dwWritten, NULL);
   //FlushFileBuffers(g_hChildStd_IN_Wr);
   //printf(chBuf);
   fflush(stdout);
   FILE *file;
   check_again:
   fflush(stdout);
    if (file = fopen("C:\\Users\\berks\\Scenic\\examples\\carla\\Carla_Challenge\\gen.scenic", "r")) 
    {
        fclose(file);
        printf("file exists");
    }
    else
    {
        printf("file doesn't exist");
        goto check_again;
    }
   Sleep(10000);
   int l = strlen("scenic 'C:\\Users\\berks\\Scenic\\examples\\carla\\Carla_Challenge\\gen.scenic' --simulate -b\n");
   memcpy(chBuf,"scenic 'C:\\Users\\berks\\Scenic\\examples\\carla\\Carla_Challenge\\gen.scenic' --simulate -b\n",l+1);
   WriteFile(g_hChildStd_IN_Wr, chBuf,l+1, &dwWritten, NULL);
   FlushFileBuffers(g_hChildStd_IN_Wr);
   if ( ! CloseHandle(g_hChildStd_IN_Wr) ) 
      ErrorExit(TEXT("StdInWr CloseHandle")); 
   printf("CHUCHU\n");
   printf(chBuf);
   printf("CHUCHU\n");
   fflush(stdout);
   ReadFromPipe();
  /* while(1){
       Sleep(10000);
   }*/
// Close the pipe handle so the child process stops reading. 
 
  
} 
 
void ReadFromPipe(void) 

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{ 
   DWORD dwRead, dwWritten; 
   CHAR chBuf[BUFSIZE]; 
   BOOL bSuccess = FALSE;
   HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

   for (;;) 
   { 
      bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
      if( ! bSuccess ) break; 

      /*bSuccess = WriteFile(hParentStdOut, chBuf, 
                           dwRead, &dwWritten, NULL);
      if (! bSuccess ) break; */
      printf(chBuf);
      printf("\n");
      fflush(stdout);
      Sleep(10000);
   } 
} 
 
void ErrorExit(PTSTR lpszFunction) 

// Format a readable error message, display a message box, 
// and exit from the application.
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    sprintf((LPTSTR)lpDisplayBuf, 
        "%s failed with error %d: %s", 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}
void eprintf(HANDLE hFile, const char* s,DWORD* d){
    WriteFile(
      hFile,            // Handle to the file
      s,  // Buffer to write
      strlen(s),   // Buffer size
      d,    // Bytes written
      NULL);         // Overlapped
}
void scenic_file_write(struct var_arr* cars,struct var_arr* objs,uint8_t ego){
    /*
    FILE* scenic_file;
    scenic_file = fopen(".\\examples\\carla\\Carla_Challenge\\gen.scenic","w");
    fprintf(scenic_file,"param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')\n");
    fprintf(scenic_file,"param carla_map = 'Town01'\n");
    fprintf(scenic_file,"model scenic.simulators.carla.model\n");
    fprintf(scenic_file,"\n\n");
    fprintf(scenic_file,"EGO_MODEL = \"vehicle.audi.a2\"\n");
    fprintf(scenic_file,"behavior GoForward(speed):\n\ttake SetSpeedAction(speed)\n");
    fprintf(scenic_file,"lane = Uniform(*network.lanes)\n");
    fprintf(scenic_file,"spot = OrientedPoint on lane.centerline\n");
    int16_t x_ego = cars->x[ego];
    int16_t y_ego = cars->y[ego];
    for(int i =0; i<cars->size;i++){
        if(i == ego){
            continue;
        }
        char spot_line[256];
        int16_t x = (cars->x[i]-x_ego)/5;
        float y = (float)(cars->y[i]-y_ego)/50.0;
        if(x < 10 && x >= 0){ x = 10; }
        if(x > -10 && x < 0) {x = -10;}
        if(y > 2.5){ y = 2.5; }
        if(y < -2.5){ y = -2.5; }
        sprintf(spot_line,"spot_%d = OrientedPoint at %f @ %d relative to spot,\n\twith heading %d deg relative to spot.heading\n",i, y,x,cars->heading[i]);
        fprintf(scenic_file,spot_line);
    }
    fprintf(scenic_file,"\n\n");
    for(int i =0; i<objs->size;i++){
        char spot_line[256];
        int16_t x = (objs->x[i]-x_ego)/5;
        float y = (float)(objs->y[i]-y_ego)/50.0;
        if(x < 10 && x >= 0){ x = 10; }
        if(x > -10 && x < 0) {x = -10;}
        if(y > 2.5){ y = 2.5; }
        if(y < -2.5){ y = -2.5; }
        sprintf(spot_line,"ospot_%d = OrientedPoint at %f @ %d relative to spot,\n\twith heading %d deg relative to spot.heading\n",i, y,x,objs->heading[i]);
        fprintf(scenic_file,spot_line);
    }
    fprintf(scenic_file,"\n\n");
    fprintf(scenic_file,"ego = Car following roadDirection from spot for 0,\n");
    fprintf(scenic_file,"\twith blueprint EGO_MODEL,\n\twith behavior AutopilotBehavior()\n");
    fprintf(scenic_file,"\n\n");
    for(int i =0; i<cars->size;i++){
        if(i == ego){
            continue;
        }
        char car_line[256];
        if(cars->state[i]&AUTOPILOT){
            sprintf(car_line,"car_%d = Car at spot_%d,\n\twith heading 0 deg relative to spot_%d.heading,\n\twith behavior AutopilotBehavior()\n",i,i,i);
        }
        else{
            sprintf(car_line,"car_%d = Car at spot_%d,\n\twith heading 0 deg relative to spot_%d.heading,\n\twith behavior GoForward(%d)\n",i,i,i,cars->speed[i]);
        }
        fprintf(scenic_file,car_line);
    }
    fprintf(scenic_file,"\n\n");
    for(int i =0; i<objs->size;i++){
        char obj_line[256];
        sprintf(obj_line,"obj_%d = Trash at ospot_%d,\n\twith heading 0 deg relative to ospot_%d.heading\n",i,i,i);
        
        fprintf(scenic_file,obj_line);
    }
    fprintf(scenic_file,"\n\n");
    fprintf(scenic_file,"terminate after 120 seconds\n");
    fprintf(scenic_file,"terminate when withinDistanceToAnyObjs(ego,7) and ego.speed < 0.01\n");
    fflush(scenic_file);
    
    fclose(scenic_file);
    printf("Hello There");
    */
    //FILE* scenic_file;//scenic_file = fopen(".\\examples\\carla\\Carla_Challenge\\gen.scenic","w");
     HANDLE hFile = CreateFile(
      "C:\\Users\\berks\\Scenic\\examples\\carla\\Carla_Challenge\\gen.scenic",     // Filename
      GENERIC_WRITE|GENERIC_READ,          // Desired access
      FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share mode
      NULL,                   // Security attributes
      CREATE_ALWAYS,             // Creates a new file, only if it doesn't already exist
      FILE_ATTRIBUTE_NORMAL,  // Flags and attributes
      NULL);                  // Template file handle
  //    fclose(scenic_file);
      if (hFile == INVALID_HANDLE_VALUE)
   {
       printf("?\n");
       fflush(stdout);
   }
    DWORD bytesWritten;
    printf("%d\n",GetLastError());
    fflush(stdout);
    eprintf(hFile,"param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')\n",&bytesWritten);
    eprintf(hFile,"param carla_map = 'Town01'\n",&bytesWritten);
    eprintf(hFile,"model scenic.simulators.carla.model\n",&bytesWritten);
    eprintf(hFile,"\n\n",&bytesWritten);
    eprintf(hFile,"EGO_MODEL = \"vehicle.audi.a2\"\n",&bytesWritten);
    eprintf(hFile,"behavior GoForward(speed):\n\ttake SetSpeedAction(speed)\n",&bytesWritten);
    eprintf(hFile,"lane = Uniform(*network.lanes)\n",&bytesWritten);
    eprintf(hFile,"spot = OrientedPoint on lane.centerline\n",&bytesWritten);
    int16_t x_ego = cars->x[ego];
    int16_t y_ego = cars->y[ego];
    for(int i =0; i<cars->size;i++){
        if(i == ego){
            continue;
        }
        char spot_line[256];
        int16_t x = (cars->x[i]-x_ego)/5;
        float y = (float)(cars->y[i]-y_ego)/50.0;
        if(x < 10 && x >= 0){ x = 10; }
        if(x > -10 && x < 0) {x = -10;}
        if(y > 2.5){ y = 2.5; }
        if(y < -2.5){ y = -2.5; }
        sprintf(spot_line,"spot_%d = OrientedPoint at %f @ %d relative to spot,\n\twith heading %d deg relative to spot.heading\n",i, y,x,cars->heading[i]);
        eprintf(hFile,spot_line,&bytesWritten);
    }
    eprintf(hFile,"\n\n",&bytesWritten);
    for(int i =0; i<objs->size;i++){
        char spot_line[256];
        int16_t x = (objs->x[i]-x_ego)/5;
        float y = (float)(objs->y[i]-y_ego)/50.0;
        if(x < 10 && x >= 0){ x = 10; }
        if(x > -10 && x < 0) {x = -10;}
        if(y > 2.5){ y = 2.5; }
        if(y < -2.5){ y = -2.5; }
        sprintf(spot_line,"ospot_%d = OrientedPoint at %f @ %d relative to spot,\n\twith heading %d deg relative to spot.heading\n",i, y,x,objs->heading[i]);
        eprintf(hFile,spot_line,&bytesWritten);
    }
    eprintf(hFile,"\n\n",&bytesWritten);
    eprintf(hFile,"ego = Car following roadDirection from spot for 0,\n",&bytesWritten);
    eprintf(hFile,"\twith blueprint EGO_MODEL,\n\twith behavior AutopilotBehavior()\n",&bytesWritten);
    eprintf(hFile,"\n\n",&bytesWritten);
    for(int i =0; i<cars->size;i++){
        if(i == ego){
            continue;
        }
        char car_line[256];
        if(cars->state[i]&AUTOPILOT){
            sprintf(car_line,"car_%d = Car at spot_%d,\n\twith heading 0 deg relative to spot_%d.heading,\n\twith behavior AutopilotBehavior()\n",i,i,i);
        }
        else{
            sprintf(car_line,"car_%d = Car at spot_%d,\n\twith heading 0 deg relative to spot_%d.heading,\n\twith behavior GoForward(%d)\n",i,i,i,cars->speed[i]);
        }
        eprintf(hFile,car_line,&bytesWritten);
    }
    eprintf(hFile,"\n\n",&bytesWritten);
    for(int i =0; i<objs->size;i++){
        char obj_line[256];
        sprintf(obj_line,"obj_%d = Trash at ospot_%d,\n\twith heading 0 deg relative to ospot_%d.heading\n",i,i,i);
        
        eprintf(hFile,obj_line,&bytesWritten);
    }
    eprintf(hFile,"\n\n",&bytesWritten);
    eprintf(hFile,"terminate after 120 seconds\n",&bytesWritten);
    eprintf(hFile,"terminate when withinDistanceToAnyObjs(ego,7) and ego.speed < 0.01\n",&bytesWritten);
    FlushFileBuffers(hFile);
    printf("WTF");
    fflush(stdout);
    CloseHandle(hFile);
}








int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screen_width = 1000;
    const int screen_height = 800;
    const int button_width = 100;
    const int button_height = 50;
    uint8_t add_car = 0;
    uint8_t add_object = 0;
    uint8_t select_as_ego = 0;
    uint8_t has_selection = 0;
    uint8_t toggle_curb = 0;
    uint8_t toggle_noise = 0;
    uint8_t toggle_auto = 0;
    uint8_t gen_script = 0;
    uint8_t current_car_index = NONE_INDEX;
    uint8_t ego_car_index = NONE_INDEX;
    
    Vector2 mouse = {-1,-1};
    
    struct var_arr cars;
    struct var_arr objects;
    init_arr(&cars);
    init_arr(&objects);
    
    InitWindow(screen_width, screen_height, "Visual Scene Editor");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 old_mouse;
            Vector2 tmp_mouse;
            tmp_mouse = GetMousePosition();
            if(tmp_mouse.x < screen_width-15 && tmp_mouse.x > 15 && tmp_mouse.y < 300 && tmp_mouse.y > 50){
                old_mouse = mouse;
                mouse = tmp_mouse;
                has_selection = !(has_selection && is_inside(mouse,old_mouse));
            }
            
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            // 15, 350      **      300, 350      **      575, 350
            // 15, 450      **      300, 450      **      575, 450
            add_car = GuiButton((Rectangle){ 15, 350, button_width, button_height }, "ADD CAR");
            add_object = GuiButton((Rectangle){ 15, 450, button_width, button_height }, "ADD OBJECT");
            select_as_ego = GuiButton((Rectangle){ 300, 350, button_width, button_height }, "SET AS EGO");
            toggle_curb = GuiButton((Rectangle){ 575, 350, button_width, button_height }, "TOGGLE ROAD");
            toggle_noise = GuiButton((Rectangle){ 575, 450, button_width, button_height }, "TOGGLE NOISE");
            toggle_auto = GuiButton((Rectangle){ 575, 550, button_width, button_height }, "TOGGLE AUTOPILOT");
            gen_script = GuiButton((Rectangle){ 15, 600, button_width, button_height }, "RUN SCENE");
            if(GuiButton((Rectangle){ 300, 450, button_width/2, button_height }, "<") && current_car_index && current_car_index != NONE_INDEX){
                current_car_index--;
            }
            if(GuiButton((Rectangle){ 300+button_width/2, 450, button_width/2, button_height }, ">") && ((int)current_car_index < (int)(cars.size-1)) && current_car_index != NONE_INDEX){
                current_car_index++;
            }
            
            DrawRectangle(15,50, screen_width-30,250,WHITE);
            for(int i = 0; i < cars.size; i++){
                
                if(ego_car_index == i){
                    DrawCircle(cars.x[i],cars.y[i],10,GREEN);
                    
                }
                else{
                    DrawCircle(cars.x[i],cars.y[i],10,RED);
                }
                if(current_car_index == i){
                    DrawCircle(cars.x[i],cars.y[i],5,WHITE);
                }
            }
            for(int i = 0; i < objects.size; i++){
                DrawCircle(objects.x[i],objects.y[i],10,PURPLE);
            }
            if(has_selection){
                DrawCircle(mouse.x,mouse.y,10,BLUE);
                if(add_car){
                    current_car_index = cars.size;
                    add_car = 0;
                    add_to_arr(&cars,mouse.x,mouse.y,0,AUTOPILOT);
                    has_selection = 0;
                } else if(add_object){
                    add_object = 0;
                    add_to_arr(&objects,mouse.x,mouse.y,0,AUTOPILOT);
                    has_selection = 0;
                }
            }
            if(select_as_ego){
                if(current_car_index == ego_car_index){
                    ego_car_index = NONE_INDEX;
                }else{
                    ego_car_index = current_car_index;
                }
                select_as_ego = 0;
            }
            if(current_car_index != NONE_INDEX){
                if(toggle_noise) cars.state[current_car_index] ^= NOISE;
                if(toggle_curb)  cars.state[current_car_index] ^= CURB;
                if(toggle_auto)  cars.state[current_car_index] ^= AUTOPILOT;
                if(cars.state[current_car_index]&CURB){
                    DrawText("ON THE CURB",300,600,32,GRAY);
                }else{
                    DrawText("ON THE ROAD",300,600,32,GRAY);
                }
                if(cars.state[current_car_index]&NOISE){
                    DrawText("NOISE ON",300,650,32,GRAY);
                }else{
                    DrawText("NOISE OFF",300,650,32,GRAY);
                }
                if(!(cars.state[current_car_index]&AUTOPILOT)){
                    cars.speed[current_car_index]=GuiSlider((Rectangle){ 65, 10, button_width*3, 30 },"SPEED -","SPEED +",cars.speed[current_car_index],-100,100);
                    cars.heading[current_car_index]= GuiSlider((Rectangle){ 480, 10, button_width*3, 30 },"YAW -","YAW +",cars.heading[current_car_index],0,359);
                    char speed[16];
                    char heading[16];
                    sprintf(speed,"SPEED: %d",cars.speed[current_car_index]);
                    sprintf(heading,"YAW: %d",cars.heading[current_car_index]);
                    DrawText(speed,300,700,32,GRAY);
                    DrawText(heading,300,750,32,GRAY);
                }
            }
            if(gen_script){
                scenic_file_write(&cars,&objects,ego_car_index);
                interaction();
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
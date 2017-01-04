/////VIEW TYPE CONST
#define VIEW_TYPE_NUM                   8
#define VIEW_TYPE_NOTPROCESS_NUM		6

#define VIEW_2D_SHAPE_INDEX             0
#define VIEW_2D_PROCESS_INDEX           7
#define VIEW_2D_FORCAST_INDEX           5
#define VIEW_3D_FILE_INDEX              3
#define VIEW_3D_PROCESS_INDEX           6
#define VIEW_3D_DESIGN_INDEX            1
#define VIEW_3D_FORCAST_INDEX           2
#define VIEW_3D_EXTREME_INDEX           4

#define VIEW_2D_SHAPE                   0x00101010
#define VIEW_2D_PROCESS                 0x00111017
#define VIEW_2D_FORCAST                 0x00101015
#define VIEW_3D_FILE                    0x00001023
#define VIEW_3D_PROCESS                 0x00111126
#define VIEW_3D_DESIGN                  0x10001121
#define VIEW_3D_FORCAST                 0x00101122
#define VIEW_3D_EXTREME                 0x10101124



#define VIEW_2D                         0x00000010//////////窗口是平面显示
#define VIEW_3D                         0x00000020//////////窗口是三维显示
#define VIEW_NEED_MATANDLIGHT           0x00000100//////////是否要光照和材质
#define VIEW_GL_MULTITHREAD				0x00001000//////////是否多线程
#define VIEW_PROCESS					0x00010000//////////是否是加工过程窗口


#define VIEW_NOT_ORIGINAL				0x00100000
#define VIEW_USE_LIST					0x01000000
#define VIEW_USE_TESS					0x10000000
//////////////

/*
窗口绘图方法分为：
1直接绘制，2多线程直接绘制，3显示列表绘制，4多线程列表绘制，
以及5平时不绘制的 加工过程绘制
当绘制方式为5时不能换为别的方式。
1，2,3，4之间可以互换
加工过程窗口在平时主线程中绘图时，只清空，
其它窗口在平时无操作，生成列表和tess时用hrc，多线程时用mtgl，在窗口变化时使用mtgl;

*/

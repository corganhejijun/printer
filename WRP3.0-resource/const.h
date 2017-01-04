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



#define VIEW_2D                         0x00000010//////////������ƽ����ʾ
#define VIEW_3D                         0x00000020//////////��������ά��ʾ
#define VIEW_NEED_MATANDLIGHT           0x00000100//////////�Ƿ�Ҫ���պͲ���
#define VIEW_GL_MULTITHREAD				0x00001000//////////�Ƿ���߳�
#define VIEW_PROCESS					0x00010000//////////�Ƿ��Ǽӹ����̴���


#define VIEW_NOT_ORIGINAL				0x00100000
#define VIEW_USE_LIST					0x01000000
#define VIEW_USE_TESS					0x10000000
//////////////

/*
���ڻ�ͼ������Ϊ��
1ֱ�ӻ��ƣ�2���߳�ֱ�ӻ��ƣ�3��ʾ�б���ƣ�4���߳��б���ƣ�
�Լ�5ƽʱ�����Ƶ� �ӹ����̻���
�����Ʒ�ʽΪ5ʱ���ܻ�Ϊ��ķ�ʽ��
1��2,3��4֮����Ի���
�ӹ����̴�����ƽʱ���߳��л�ͼʱ��ֻ��գ�
����������ƽʱ�޲����������б��tessʱ��hrc�����߳�ʱ��mtgl���ڴ��ڱ仯ʱʹ��mtgl;

*/

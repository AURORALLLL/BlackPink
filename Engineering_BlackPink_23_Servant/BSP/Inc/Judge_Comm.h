#ifndef _JUDGE_COMM_H__
#define _JUDGE_COMM_H__


#include "sysconfig.h"
#include "CRC_Check.h"

#define REFEREE_DMA_SIZE (130)

#define REFEREE_HUART huart6

//���¶������<����ϵͳ����Э�鸽¼>
/*--------------CmdID(2-Byte)----------------*/
#define GAME_STATE              0X0001          //����״̬���ݣ�1Hz ���ڷ���
#define GAME_RESULT             0X0002          //����������ݣ�������������
#define ROBOT_HP                0X0003          //����������Ѫ�����ݣ�1Hz���ڷ���
#define DART_STATUS             0X0004          //���ڷ���״̬�����ڷ������
//0X0005        //�˹�������ս���ӳ���ͷ�״̬��1Hz���ڷ��͡���ʱ�ò�������ûд
#define EVENT_DATA              0X0101          //�����¼����ݣ��¼��ı����
#define SUPPLY_ACTION           0X0102          //���ز���վ������ʶ���ݣ������ı����
//0X0103        //���󲹸�վ�������ݣ��ɲ����ӷ��ͣ����� 10Hz��RM�Կ�����δ���ţ���ûд
#define REFEREE_WARNING         0X0104          //���о������ݣ����淢������ 
#define DART_REMAINING_TIME     0X0105          //���ڷ���ڵ���ʱ��1Hz ���ڷ��� 
#define GAME_ROBOT_STATE        0X0201          //������״̬���ݣ�10Hz ���ڷ���
#define POWER_HEAT_DATA         0X0202          //ʵʱ�����������ݣ�50Hz ���ڷ���
#define GAME_ROBOT_POS          0X0203          //������λ�����ݣ�10Hz ����
#define BUFF_MUSK               0X0204          //�������������ݣ�����״̬�ı����
#define AERIAL_ROBOT_ENERGY     0X0205          //���л���������״̬���ݣ�10Hz ���ڷ��ͣ�ֻ�п��л��������ط���
#define ROBOT_HURT              0X0206          //�˺�״̬���ݣ��˺���������
#define SHOOT_DATA              0X0207          //ʵʱ������ݣ��ӵ��������
#define REMAIN_BULLET           0X0208          //�ӵ�ʣ�෢���������л������Լ��ڱ������˷��ͣ�1Hz���ڷ���
#define RFID_STATE              0X0209          //������ RFID ״̬��1Hz ���ڷ���
#define DART_CLIENT_CMD         0X020A          //���ڻ����˿ͻ���ָ�����ݣ�10Hz���ڷ���
#define INTERACTIVE_HEADER      0X0301          //�����˼佻�����ݣ����ͷ��������ͣ�����10Hz
//#define DIY_CONTROL             0X0302          //�Զ���������������ݽӿڣ�ͨ���ͻ��˴������ͣ�����30Hz//���ݳ����Զ�����û��ͳһд�ˡ���Ҫ�õ��ı����������
//#define MINI_MAP                0X0303          //�ͻ���С��ͼ�������ݣ���������//Ŀǰֻ���Զ�������С��ͼ����û��ͳһд�ˡ��õ�ʱ���������
#define VIDEO_TRANSMITTER       0X0304          //���̡������Ϣ��ͨ��ͼ�����ڷ���
/*--------------CmdID(2-Byte)----------------*/

/*--------------DataSize----------------*/
#define GAME_STATE_DATA_SIZE            (11)
#define GAME_RESULT_DATA_SIZE           (1)
#define ROBOT_HP_DATA_SIZE              (32)
#define DART_STATUS_DATE_SIZE           (3)
#define EVENTDATA_DATA_SIZE             (4)
#define SUPPLY_ACTION_DATA_SIZE         (4)
#define REFEREE_WARNING_DATA_SIZE       (2)
#define DART_REMAINING_TIME_DATA_SIZE   (1)
#define GAMEROBOT_STATE_DATA_SIZE       (27)
#define POWER_HEAT_DATA_SIZE            (16)
#define GAME_ROBOT_POS_DATA_SIZE        (16)
#define BUFF_DATA_SIZE                  (1)
#define AERIAL_ROBOT_ENERGY_DATA_SIZE   (1)
#define ROBOT_HURT_DATA_SIZE            (1)
#define SHOOTDATA_DATA_SIZE             (7)
#define REMAIN_BULLET_DATA_SIZE         (6)
#define RFID_STATE_DATA_SIZE            (4)
#define DART_CLIENT_CMD_DATA_SIZE       (12)
//#define DIY_CONTROL_DATA_SIZE           (?)//����30�����Զ�
//#define MINI_MAP_DATA_SIZE              (15)
#define VIDEO_TRANSMITTER_DATA_SIZE     (12)

#define INTERACTIVEHEADER_DATA_SIZE(n) (n + 9)
#define JUDGE_DATA_LENGTH(n) (n + 9)
/*--------------DataSize----------------*/

/*--------------ƫ��λ��----------------*/
//��������
#define JUDGE_SOF_OFFSET (0)
#define JUDGE_DATALENGTH_OFFSET (1)
#define JUDGE_SEQ_OFFSET (3)
#define JUDGE_CRC8_OFFSET (4)
#define JUDGE_CMDID_OFFSET (5)
#define JUDGE_DATA_OFFSET (7)
#define JUDGE_CRC16_OFFSET(n) (n + JUDGE_DATA_OFFSET)
//��������
#define TRAMSINIT_LENGTH 128
#define TRAMSINIT_HEAD_OFFSET 0
#define TRAMSINIT_SENDID_OFFSET 2
#define TRAMSINIT_CLIENT_OFFSET 4
/*--------------ƫ��λ��----------------*/


//1.	����������״̬(0x0001)
typedef __packed struct
{
    /*
    0-3 bit����������
    ? 1��RoboMaster ���״�ʦ����
    ? 2��RoboMaster ���״�ʦ��������
    ? 3��ICRA RoboMaster �˹�������ս��
    ? 4��RoboMaster ������3V3
    ? 5��RoboMaster ������1V1
    */
    uint8_t game_type : 4;

    /*
    4-7 bit����ǰ�����׶�
    ? 0��δ��ʼ������
    ? 1��׼���׶Σ�
    ? 2���Լ�׶Σ�
    ? 3��5s����ʱ��
    ? 4����ս�У�
    ? 5������������
    */
    uint8_t game_progress : 4;

    /*
    ��ǰ�׶�ʣ��ʱ�䣬��λ s
    */
    uint16_t stage_remain_time;

    /*
    �����˽��յ���ָ��ľ�ȷUnixʱ�䣬�����ض��յ���Ч��NTP��������ʱ����Ч
    */
   uint64_t SyncTimeStamp;

} ext_game_state_t;

//2.����������ݣ�0x0002������Ƶ�ʣ�������������
typedef __packed struct
{
    /*0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��*/
    uint8_t winner;
} ext_game_result_t;

//3. ������Ѫ�����ݣ�0x0003������Ƶ�ʣ�1Hz
typedef __packed struct
{
    uint16_t red_1_robot_HP;    
    uint16_t red_2_robot_HP;    
    uint16_t red_3_robot_HP;    
    uint16_t red_4_robot_HP;    
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;    
    uint16_t red_outpost_HP;   
    uint16_t red_base_HP;    

    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;    
    uint16_t blue_3_robot_HP;    
    uint16_t blue_4_robot_HP;    
    uint16_t blue_5_robot_HP;    
    uint16_t blue_7_robot_HP;    
    uint16_t blue_outpost_HP;   
    uint16_t blue_base_HP; 
} ext_game_robot_HP_t;

//4. ���ڷ���״̬��0x0004������Ƶ�ʣ����ڷ�����ͣ����ͷ�Χ�����л����ˡ� 
typedef __packed struct 
{   /*
    ������ڵĶ��飺
    1���췽����
    2����������
    */
    uint8_t dart_belong;    
    /*
    ����ʱ��ʣ�����ʱ�䣬��λs
    */
    uint16_t stage_remaining_time;  
} ext_dart_status_t; 

//5.�˹�������ս���ӳ���ͷ���״̬��0x0005������Ƶ�ʣ�1Hz���ڷ��ͣ����ͷ�Χ�����л�����
//��ʱ�ò�������ûд

//6.�����¼����ݣ�0x0101������Ƶ�ʣ��¼��ı����
typedef __packed struct
{
    /*
    bit 0-2��
    bit 0����������վ 1�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ�죻
    bit 1����������վ 2�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ�죻
    bit 2����������վ 3�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ�죻

    bit 3-5��������������״̬��
    ? bit 3Ϊ�����ռ��״̬��1Ϊռ�죻
    ? bit 4ΪС�������ؼ���״̬��1Ϊ�Ѽ��
    ? bit 5Ϊ���������ؼ���״̬��1Ϊ�Ѽ��

    bit 6������R2���θߵ�ռ��״̬ 1Ϊ��ռ�죻
    bit 7������R3���θߵ�ռ��״̬ 1Ϊ��ռ�죻
    bit 8������R4���θߵ�ռ��״̬ 1Ϊ��ռ�죻

    bit 9���������ػ���״̬��
    ? 1 Ϊ���������⻤��Ѫ����
    ? 0Ϊ���������⻤��Ѫ����

    bit 10������ǰ��ս״̬��
    ? 1Ϊǰ��ս��
    ? 0Ϊǰ��ս�����٣�

    bit 10 -31: ����
    */
    uint32_t event_type;
} ext_event_data_t;

//7. ����վ������ʶ��0x0102������Ƶ�ʣ������ı���͡����ͷ�Χ������������
typedef __packed struct
{
    /*
    ����վ�� ID��
    1��1 �Ų����ڣ�
    2��2 �Ų�����
    */
    uint8_t supply_projectile_id;

    /*
    ���������� ID��0 Ϊ��ǰ�޻����˲�����1 Ϊ�췽Ӣ�ۻ����˲�����2 Ϊ�췽����
    �����˲�����3/4/5 Ϊ�췽���������˲�����101 Ϊ����Ӣ�ۻ����˲�����102 Ϊ����
    ���̻����˲�����103/104/105 Ϊ�������������˲���
    */
    uint8_t supply_robot_id;

    /*
    �����ڿ���״̬��0 Ϊ�رգ�1 Ϊ�ӵ�׼���У�2 Ϊ�ӵ�����
    */
    uint8_t supply_projectile_step;

    /*
    ����������
    50��50 ���ӵ���
    100��100 ���ӵ���
    150��150 ���ӵ���
    200��200 ���ӵ���
    */
    uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//8. ���о�����Ϣ��cmd_id (0x0104)������Ƶ�ʣ����淢�����ͣ����ͷ�Χ�����������ˡ� 
typedef __packed struct 
{   
    /*
    ����ȼ���
    1������
    2������
    3���и�
    */
    uint8_t level;
    /*
    ���������ID��
    �и�ʱ��������IDΪ0
    ���ơ�����ʱ��������IDΪ���������ID
    */
    uint8_t foul_robot_id;  
} ext_referee_warning_t; 

//9. ���ڷ���ڵ���ʱ��cmd_id (0x0105)������Ƶ�ʣ�1Hz ���ڷ��ͣ����ͷ�Χ�����������ˡ� 
typedef __packed struct 
{   
    uint8_t dart_remaining_time;   //15s ����ʱ 
} ext_dart_remaining_time_t; 

//10.����������״̬��0x0201������Ƶ�ʣ�10Hz
typedef __packed struct
{
    /*
    ������ ID��
    1���췽Ӣ�ۻ����ˣ�
    2���췽���̻����ˣ�
    3/4/5���췽���������ˣ�
    6���췽���л����ˣ�
    7���췽�ڱ������ˣ�
    8���췽���ڻ����ˣ�
    9���췽�״�վ��  
    101������Ӣ�ۻ����ˣ�
    102���������̻����ˣ�
    103/104/105���������������ˣ�
    106���������л����ˣ�
    107�������ڱ������ˡ�
    108���������ڻ����ˣ� 
    109�������״�վ�� 
    */
    uint8_t robot_id;

    /*
    �����˵ȼ���
    1��һ����2��������3��������
    */
    uint8_t robot_level;

    /*
    ������ʣ��Ѫ��
    */
    uint16_t remain_HP;

    /*
    ����������Ѫ��
    */
    uint16_t max_HP;

    /*
    ������1��17mmǹ��ÿ����ȴֵ
    */
    uint16_t shooter_id1_17mm_cooling_rate;

    /*
    ������1��17mmǹ����������
    */
    uint16_t shooter_id1_17mm_cooling_limit;

    /*
    ������1��17mm ǹ�������ٶ� ��λ m/s
    */
    uint16_t shooter_id1_17mm_speed_limit;

    /*
    ������2��17mmǹ��ÿ����ȴֵ
     */
    uint16_t shooter_id2_17mm_cooling_rate;

    /*
    ������2��17mmǹ����������
    */
    uint16_t shooter_id2_17mm_cooling_limit; 

    /*
    ������2��17mm ǹ�������ٶ� ��λ m/s
    */
    uint16_t shooter_id2_17mm_speed_limit;   

    /*
    ������42mmǹ��ÿ����ȴֵ
    */
    uint16_t shooter_id1_42mm_cooling_rate; 

    /*
    ������42mmǹ����������
    */
    uint16_t shooter_id1_42mm_cooling_limit;

    /*
    ������42mmǹ�������ٶ� ��λ m/s
    */
    uint16_t shooter_id1_42mm_speed_limit;

    /*
    �����˵��̹�����������
    */
    uint16_t chassis_power_limit; 

    /*
    ���ص�Դ��������
    0 bit��gimbal������� 1Ϊ��24V�����0Ϊ��24v�����
    1 bit��chassis�������1Ϊ��24V�����0Ϊ��24v�����
    2 bit��shooter�������1Ϊ��24V�����0Ϊ��24v�����
    */
    uint8_t mains_power_gimbal_output : 1; 
    uint8_t mains_power_chassis_output : 1; 
    uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

//11.ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz
typedef __packed struct
{
    //���������ѹ ��λ ����
    uint16_t chassis_volt;
    //����������� ��λ ����
    uint16_t chassis_current;
    //����������� ��λ W ��
    float chassis_power;
    //���̹��ʻ��� ��λ J ����
    uint16_t chassis_power_buffer;
    //1��17mm ǹ������
    uint16_t shooter_id1_17mm_cooling_heat;
    //2��17mmǹ������
    uint16_t shooter_id2_17mm_cooling_heat;
    //42mm ǹ������
    uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;

//12.������λ�ã�0x0203������Ƶ�ʣ�10Hz
typedef __packed struct
{
    float x;   //λ�� x ���꣬��λ m
    float y;   //λ�� y ���꣬��λ m
    float z;   //λ�� z ���꣬��λ m
    float yaw; //λ��ǹ�ڣ���λ��
} ext_game_robot_pos_t;

//13. ���������棺0x0204������Ƶ�ʣ�1Hz
typedef __packed struct
{
    /*
    bit 0��������Ѫ����Ѫ״̬
    bit 1��ǹ��������ȴ����
    bit 2�������˷����ӳ�
    bit 3�������˹����ӳ�
    ���� bit ����
    */
    uint8_t power_rune_buff;
} ext_buff_t;

//14. ���л���������״̬��0x0205������Ƶ�ʣ�10Hz
typedef __packed struct
{
    uint8_t attack_time;  //�ɹ���ʱ�� ��λ s��30s �ݼ��� 0
} aerial_robot_energy_t;

//15. �˺�״̬��0x0206������Ƶ�ʣ��˺���������
typedef __packed struct
{
    /*
    bit 0-3����Ѫ���仯����Ϊװ���˺�������װ�� ID��������ֵΪ 0-4 �Ŵ��������
    �����װ��Ƭ������Ѫ���仯���ͣ��ñ�����ֵΪ 0��
    */
    uint8_t armor_id : 4;
    /*bit 4-7��Ѫ���仯����
    0x0 װ���˺���Ѫ��
    0x1 ģ����߿�Ѫ��
    0x2 �����ٿ�Ѫ��
    0x3 ��ǹ��������Ѫ��
    0x4 �����̹��ʿ�Ѫ��
    0x5 װ��ײ����Ѫ��
    */
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//16. ʵʱ�����Ϣ��0x0207������Ƶ�ʣ��������
typedef __packed struct
{
    /*�ӵ�����: 1��17mm���� 2��42mm����*/
    uint8_t bullet_type; 
    /*
    �������ID��
    1��1��17mm�������
    2��2��17mm�������
    3��42mm �������
    */
    uint8_t shooter_id; 
    /*�ӵ���Ƶ ��λ Hz*/
    uint8_t bullet_freq; 
    /*�ӵ����� ��λ m/s*/
    float bullet_speed; 
} ext_shoot_data_t;

//17. �ӵ�ʣ�෢������0x0208������Ƶ�ʣ�10Hz���ڷ��ͣ����л����˷��� 
typedef __packed struct 
{   
    uint16_t bullet_remaining_num_17mm;//17mm�ӵ�ʣ�෢����Ŀ
    uint16_t bullet_remaining_num_42mm;//42mm�ӵ�ʣ�෢����Ŀ
    uint16_t coin_remaining_num;//ʣ��������
} ext_bullet_remaining_t;

//18. ������ RFID ״̬��0x0209������Ƶ�ʣ�1Hz�����ͷ�Χ����һ�����ˡ� 
typedef __packed struct 
{   
    /*
    bit 0����������� RFID ״̬�� 
    bit 1���ߵ������ RFID ״̬�� 
    bit 2���������ؼ���� RFID ״̬�� 
    bit 3����������� RFID ״̬�� 
    bit 4��ǰ�ڸ������ RFID ״̬�� 
    bit 5����Դ������� RFID ״̬�� 
    bit 6����Ѫ������� RFID ״̬�� 
    bit 7�����̻����˲�Ѫ�� RFID ״̬�� 
    bit 8-31������
    RFID ״̬����ȫ�����Ӧ������򴦷�״̬������з���ռ��ĸߵ�����㣬���ܻ�ȡ��Ӧ������Ч����
    */
    uint32_t rfid_status;
} ext_rfid_status_t; 

//19. ���ڻ����˿ͻ���ָ�����ݣ�0x020A������Ƶ�ʣ�10Hz�����ͷ�Χ����һ������
typedef __packed struct 
{ 
    /*
    ��ǰ���ڷ���ڵ�״̬
    0���رգ�
    1�����ڿ������߹ر���
    2���Ѿ�����
    */
    uint8_t dart_launch_opening_status;
    /*
    ���ڵĴ��Ŀ�꣬Ĭ��Ϊǰ��վ��
    1��ǰ��վ��
    2�����ء�
    */
    uint8_t dart_attack_target;
    /*�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ�룬��δ�л�Ĭ��Ϊ0��*/
    uint16_t target_change_time;
    /*��⵽�ĵ�һö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ0��*/
    uint8_t first_dart_speed;
    /*��⵽�ĵڶ�ö�����ٶȣ���λ 0.1m/s/LSB��δ�����Ϊ0��*/
    uint8_t second_dart_speed;
    /*��⵽�ĵ���ö�����ٶȣ���λ 0.1m/s/LSB��δ�����Ϊ0��*/
    uint8_t third_dart_speed;
    /*��⵽�ĵ���ö�����ٶȣ���λ 0.1m/s/LSB��δ�����Ϊ0��*/
    uint8_t fourth_dart_speed;
    /*���һ�εķ�����ڵı���ʣ��ʱ�䣬��λ�룬��ʼֵΪ0��*/
    uint16_t last_dart_launch_time;
    /*���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��, ��ʼֵΪ0��*/
    uint16_t operate_launch_cmd_time; 
} ext_dart_client_cmd_t;

//20.ͼ��ң����Ϣ��ʶ��0x0304������Ƶ�ʣ�30Hz��
//2021�����������ܣ���˵��ң������·�ȶ���û�ù�
typedef __packed struct
{
    /*���X����Ϣ*/
    int16_t mouse_x;
    /*���Y����Ϣ*/
    int16_t mouse_y;
    /*��������Ϣ*/
    int16_t mouse_z;
    /*����������*/
    int8_t left_button_down;
    /*����Ҽ�����*/
    int8_t right_button_down;
    /*
    ������Ϣ
    bit 0������W�Ƿ���
    bit 1������S�Ƿ���
    bit 2������A�Ƿ���
    bit 3������D�Ƿ���
    bit 4������SHIFT�Ƿ���
    bit 5������CTRL�Ƿ���
    bit 6������Q�Ƿ���
    bit 7������E�Ƿ���
    bit 8������R�Ƿ���
    bit 9������F�Ƿ���
    bit 10������G�Ƿ���
    bit 11������Z�Ƿ���
    bit 12������X�Ƿ���
    bit 13������C�Ƿ���
    bit 14������V�Ƿ���
    bit 15������B�Ƿ���
    */
    uint16_t keyboard_value;
    /*����λ*/
    uint16_t reserved;
} ext_video_transmitter_t;


extern ext_game_state_t ext_game_state;
extern ext_game_result_t ext_game_result;
extern ext_game_robot_HP_t ext_game_robot_HP;
extern ext_dart_status_t ext_dart_status;
extern ext_event_data_t ext_event_data;
extern ext_supply_projectile_action_t ext_supply_projectile_action;
extern ext_referee_warning_t ext_referee_warning;
extern ext_dart_remaining_time_t ext_dart_remaining_time;
extern ext_game_robot_state_t ext_game_robot_state;
extern ext_power_heat_data_t ext_power_heat_data;
extern ext_game_robot_pos_t ext_game_robot_pos;
extern ext_buff_t ext_buff;
extern aerial_robot_energy_t aerial_robot_energy;
extern ext_robot_hurt_t ext_robot_hurt;
extern ext_shoot_data_t ext_shoot_data;
extern ext_bullet_remaining_t ext_bullet_remaining;
extern ext_rfid_status_t ext_rfid_status;
extern ext_dart_client_cmd_t ext_dart_client_cmd;
extern ext_video_transmitter_t ext_video_transmitter;

void BSP_Init_JudgeComm(void);
void Referee_IDLECallback(UART_HandleTypeDef *huart);
void RefereeReceive(uint8_t JudgeReceive_Counter);
void Referee_Receive_Data_Processing(uint8_t SOF, uint16_t CmdID);

#endif


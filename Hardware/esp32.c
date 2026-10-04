#include "my_usart.h"
#include "esp32.h"

ESP_Typedef esp = { 0 };

/**
 * @brief WiFi连接函数
 * 
 * @param ssid WiFi名称
 * @param pwd WiFi密码
 * @return 返回0 连接成功，返回-1连接失败
 */
int8_t wifi_Connect(const char* ssid, const char* pwd)
{
    // 设置为STA模式   AT+CWMODE=1
    int8_t rst = -1;
    uint8_t cmd_buf[256] = {0};
    rst = atCmdSend((uint8_t*)"AT+CWMODE=1\r\n", "OK", 2000);
    if (rst != 0)
    {
        printf("设置sta模式失败\r\n");
        return -1;
    }
    // 连接WIFI
    snprintf((char*)cmd_buf, 256, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    rst = atCmdSend(cmd_buf, "OK", 20000);
    if(rst != 0) { 
        printf("wifi连接失败\r\n"); 
        esp.isWifiConnect = 0; 
        return -1; 
    }
    esp.isWifiConnect = 1;
    return 0;
}

/**
 * @brief 发送AT指令并接收指定的字符串
 * 
 * @param cmd 要发送的At指令
 * @param ack 要接收的指定字符串
 * @param waitTime 超时实现
 * @return int8_t 发送成功返回0， 发送失败返回-1
 */
int8_t atCmdSend(uint8_t* cmd, const char* ack, u16 waitTime)
{
    u2.isReady = 0;
    u2.count = 0;
    memset(u2.buf, 0, sizeof u2.buf);
    uart2_sendString(cmd);
    while(waitTime--)
    {
        if(strstr((const char*)u2.buf, ack) != NULL)
        {
            return 0;
        }
        Delay_ms(1);
    }
    // 超时没有找到指定的字符串
    printf("{%s 超时}\r\n", cmd);
    return -1;
}

/**
 * @brief 通过IP和端口号连接指定的服务器，并进入透传模式
 * 
 * @param ip 
 * @param port 
 * @return int8_t 成功返回0 失败返回-1
 */
int8_t wifi_Connect_Server(const char* ip, int port)
{
    int ret = -1;
    uint8_t cmd_buf[256] = {0};
    // ret = atCmdSend("AT+CIPSTART=\"TCP\",\"api.pinduoduo.com\",80\r\n", "OK", 5500);
    snprintf((char*)cmd_buf, 256, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);
    ret = atCmdSend(cmd_buf, "OK", 7000);
    if(ret == -1) { printf("服务器连接失败\r\n"); esp.isServerConnect = 0; return -1;}
    // AT+CIPMODE=1
    ret = atCmdSend("AT+CIPMODE=1\r\n", "OK", 3000);
    ret = atCmdSend("AT+CIPSEND\r\n", ">", 3000);
    if(ret == -1) { printf("进入透传失败\r\n"); esp.isServerConnect = 0; return -1;}
    esp.isServerConnect = 1;
    return 0;
}
/**
 * @brief 返回WiFi的连接状态
 * 
 * @param ssid 要查询是否连接的WiFi名
 * @return int8_t 返回-1未连接  返回0已连接
 */
int8_t wifi_GetState(const char* ssid)
{
    int ret = -1;
    ret = atCmdSend("AT+CWSTATE?\r\n", ssid, 5500);
    if(ret == -1) { return -1;}
    return 0;
}
/**
 * @brief 退出透传模式，断开TCP服务器的连接
 * 
 * @return int8_t 
 */
int8_t wifi_Disconnect_Server(void)
{
    //if(esp.isServerConnect == 1)
    //{
        int ret = -1;
        Delay_ms(1000);
        uart2_sendString("+++");
        Delay_ms(1000);
        ret = atCmdSend("AT+CIPCLOSE\r\n", "OK", 3000);
        if(ret == -1) { return -1;}
        return 0;
    //}
    //return 0;
}
/**
 * @brief 联网获取时间戳
 * 
 * @return uint32_t 新的时间戳
 */
uint32_t getNewTime(void)
{
    //wifi_Disconnect_Server();
    wifi_Connect_Server("api.pinduoduo.com", 80);
    atCmdSend("GET http://api.pinduoduo.com/api/server/_stm\r\n", "time", 6000);
    Delay_ms(200);
    cJSON *root = cJSON_Parse(u2.buf);
    cJSON *item = cJSON_GetObjectItem(root, "server_time");
    uint32_t t = (uint32_t)(item->valuedouble / 1000.0);
    wifi_Disconnect_Server();
    return t;
}


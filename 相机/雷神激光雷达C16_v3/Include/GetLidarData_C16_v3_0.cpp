#include "GetLidarData_C16_v3_0.h"

#define ConversionDistance_C16_v3_0        4.319 / 100.0        //光学中心转结构中心的 距离 （4.319 单位为 厘米） 转成  米为单位
#define ConversionAngle_C16_v3_0        14.68                //光学中心转结构中心的 角度  单位为 度
#define IDistanceACC_C16_v3_0            0.25 / 100.0            //距离精度转换	 0.25cm	转换成 1m			距离转换成 米为单位

GetLidarData_C16_v3_0::GetLidarData_C16_v3_0() {
    float theta[16] = {-15, 1, -13, 3, -11, 5, -9, 7, -7, 9, -5, 11, -3, 13, -1, 15};
    for (int i = 0; i < 16; i++) {
        cosTheta[i] = cos(theta[i] * PI / 180);
        sinTheta[i] = sin(theta[i] * PI / 180);
    }


    BlockAngle.clear();
    all_BlockAngle.clear();
    endFrameAngle = 90.0;                    //固定在90度解析数据,
}

GetLidarData_C16_v3_0::~GetLidarData_C16_v3_0() {
}

void GetLidarData_C16_v3_0::setVertialAngles(unsigned char *data) {
    unsigned char Dev_buff[1206] = {};
    memcpy(Dev_buff, data, 1206);
    if (Dev_buff[0] == 0xa5 && Dev_buff[1] == 0xff && Dev_buff[2] == 0x00 && Dev_buff[3] == 0x5a &&
        Dev_buff[4] == 0x11 && Dev_buff[5] == 0x11 && Dev_buff[6] == 0x55 && Dev_buff[7] == 0x55) {
        //垂直角度
        for (int i = 0; i < 16; i++) {
            uint8_t data1 = Dev_buff[245 + 2 * i];
            uint8_t data2 = Dev_buff[245 + 2 * i + 1];
            int angleVal = data1 * 256 + data2;
            if (angleVal > 32767) {
                angleVal = angleVal - 65536;
            }
            vertialAngles[i] = (float) angleVal / 100.0;
        }
    }

    float theta[16] = {-15, 1, -13, 3, -11, 5, -9, 7, -7, 9, -5, 11, -3, 13, -1, 15};
    bool flag_err = false;
    for (int i = 0; i < 16; i++) {
        //若在设备包获取的垂直角度值 和 对应的通道预设的值 相差的绝对值 大于1.5 ， 则会发错误信息， 并退出其的角度的比较
        if (abs(vertialAngles[i] - theta[i]) > 1.5) {
            flag_err = true;
            break;
        }
    }
    for (int i = 0; i < 16; i++) {
        //如果未出错  则使用获取的垂直角度 ， 否则使用预设的垂直角度（一个角度出错，都不行）
        if (!flag_err) {
            theta[i] = vertialAngles[i];
        }
        cosTheta[i] = cos(theta[i] * PI / 180);
        sinTheta[i] = sin(theta[i] * PI / 180);
    }
}

void GetLidarData_C16_v3_0::LidarRun() {
    while (true) {
        if (isQuit) {
            clearQueue(allDataValue);
            break;
        }
        if (!allDataValue.empty()) {
            unsigned char data[1206] = {0};
            m_mutex.lock();
            memcpy(data, allDataValue.front(), 1206);
            delete allDataValue.front();
            allDataValue.pop();
            m_mutex.unlock();

            if (data[0] == 0xa5 && data[1] == 0xff && data[2] == 0x00 && data[3] == 0x5a) {
                setVertialAngles(data);

                m_UTC_Time.year = data[52] + 2000;
                m_UTC_Time.month = data[53];
                m_UTC_Time.day = data[54];
                m_UTC_Time.hour = data[55];
                m_UTC_Time.minute = data[56];
                m_UTC_Time.second = data[57];
                continue;
            }

            timestamp_nsce = (data[1203] * pow(2, 24) + data[1202] * pow(2, 16) + data[1201] * pow(2, 8) +
                              data[1200] * pow(2, 0)) * pow(10, 3);
            struct tm t;
            t.tm_sec = m_UTC_Time.second;
            t.tm_min = m_UTC_Time.minute;
            t.tm_hour = m_UTC_Time.hour;
            t.tm_mday = m_UTC_Time.day;
            t.tm_mon = m_UTC_Time.month - 1;
            t.tm_year = m_UTC_Time.year - 1900;
            t.tm_isdst = 0;
            time_t _t = static_cast<uint64_t>(timegm(&t));//mktime(&t);
            if (-1 == _t) {
                perror("parse error");
            }
            allTimestamp = _t * pow(10, 9) + timestamp_nsce;

            int lidar_EchoModel = data[1204];
#pragma region    //判断单双回波解析
            if (0x37 == lidar_EchoModel) {
                handleSingleEcho(data);
            } else if (0x39 == lidar_EchoModel) {
                handleDoubleEcho(data);
            }
            lastAllTimestamp = allTimestamp;
#pragma endregion
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}


void GetLidarData_C16_v3_0::handleSingleEcho(unsigned char *data) {
    if (data[0] == 0xff && data[1] == 0xee) {
        std::vector<LidarPointData> lidardata_tmp;                                                //数据点解析结构
        lidardata_tmp.clear();
        bool isOneFrame = false;

        PointIntervalT_ns = (allTimestamp - lastAllTimestamp) * 1.0 / 384;
        for (int i = 0; i < 12; i++) {
            //提取出一圈中的方位角度值
            float AngleBlock = (data[3 + 100 * i] * 256 + data[2 + 100 * i]) / 100.f;
            if (AngleBlock >= 360.0) {
                BlockAngle.push_back(AngleBlock - 360.0);
                all_BlockAngle.push_back(AngleBlock - 360.0);
            } else {
                BlockAngle.push_back(AngleBlock);
                all_BlockAngle.push_back(AngleBlock);
            }

            if (BlockAngle.size() >= 2) {
				float mdiff = (BlockAngle.back() - BlockAngle[BlockAngle.size() - 2]);
				mdiff = mdiff >= 0 ? mdiff : mdiff + 360;

                for (int j = 0; j < 32; j++) {
                    double m_angle = (BlockAngle[BlockAngle.size() - 2] + (mdiff / 32) * j);
                    if (m_angle < 360.0)
                        m_DataT[j].H_angle = m_angle;
                    else
                        m_DataT[j].H_angle = m_angle - 360.0;

                    m_PointXYZ m_point = XYZ_calculate(m_DataT[j].ID, m_DataT[j].H_angle, m_DataT[j].Distance);
					m_DataT[j].X = m_point.x1;
					m_DataT[j].Y = m_point.y1;
					m_DataT[j].Z = m_point.z1;

                    LidarPointData mLidarPointData;

                    mLidarPointData.ID = m_DataT[j].ID;
                    mLidarPointData.X = m_point.x1;
                    mLidarPointData.Y = m_point.y1;
                    mLidarPointData.Z = m_point.z1;
                    mLidarPointData.Intensity = m_DataT[j].Intensity;


                    if (all_BlockAngle.size() > 100) {
                        if ((m_DataT[j].H_angle - endFrameAngle > 0) && abs(m_DataT[j].H_angle - endFrameAngle) < 5) {
                            lidardata_tmp.push_back(mLidarPointData);
                            isOneFrame = true;
                        } else {
							LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                        }
                    } else {
                        LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                    }
                }
            }

            for (int j = 0; j < 32; j++) {
                m_DataT[j].ID = j % 16;
                m_DataT[j].Distance =
                        (data[5 + 3 * j + 100 * i] * 256 + data[4 + 3 * j + 100 * i]) * IDistanceACC_C16_v3_0;
                m_DataT[j].Intensity = data[6 + 3 * j + 100 * i];
                m_DataT[j].Mtimestamp_nsce = lastAllTimestamp + (32 * i + j + 1) * PointIntervalT_ns;
            }

            if (i == 11) {
                float mdiff = (BlockAngle.back() - BlockAngle[BlockAngle.size() - 2]);
                mdiff = mdiff >= 0 ? mdiff : mdiff + 360;

                for (int j = 0; j < 32; j++) {
                    double m_angle = (BlockAngle.back() + (mdiff / 32) * j);
                    if (m_angle < 360.0)
                        m_DataT[j].H_angle = m_angle;
                    else
                        m_DataT[j].H_angle = m_angle - 360.0;

                    m_PointXYZ m_point = XYZ_calculate(m_DataT[j].ID, m_DataT[j].H_angle, m_DataT[j].Distance);
					m_DataT[j].X = m_point.x1;
					m_DataT[j].Y = m_point.y1;
					m_DataT[j].Z = m_point.z1;


                    LidarPointData mLidarPointData;
                    mLidarPointData.ID = m_DataT[j].ID;
                    mLidarPointData.X = m_point.x1;
                    mLidarPointData.Y = m_point.y1;
                    mLidarPointData.Z = m_point.z1;
                    mLidarPointData.Intensity = m_DataT[j].Intensity;

                    if (all_BlockAngle.size() > 100) {
                        if ((m_DataT[j].H_angle - endFrameAngle > 0) && abs(m_DataT[j].H_angle - endFrameAngle) < 5) {
                            lidardata_tmp.push_back(mLidarPointData);
                            isOneFrame = true;
                        } else {
                            LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                        }
                    } else {
                        LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                    }
                }
                BlockAngle.clear();
            }
        }


        if (isOneFrame) {

            sendLidarData();
            all_BlockAngle.clear();

            if (lidardata_tmp.size() > 0) {
                for (size_t SF = 0; SF < lidardata_tmp.size(); SF++) {
                    LidarPerFrameDatePrt_Get[lidardata_tmp[SF].ID].push_back(lidardata_tmp[SF]);
                }
            }
        }
    }
}


void GetLidarData_C16_v3_0::handleDoubleEcho(unsigned char *data) {
    if (data[0] == 0xff && data[1] == 0xee) {
        std::vector<LidarPointData> lidardata_tmp, lidardata_tmp_d;                                                //数据点解析结构
        lidardata_tmp.clear();
        lidardata_tmp_d.clear();

        bool isOneFrame = false;
        PointIntervalT_ns = (allTimestamp - lastAllTimestamp) * 1.0 / 384 / 2;

        for (int i = 0; i < 12; i += 2) {
            //提取出一圈中的方位角度值
            float AngleBlock = (data[3 + 100 * i] * 256 + data[2 + 100 * i]) / 100.f;
            if (AngleBlock >= 360.0) {
                BlockAngle.push_back(AngleBlock - 360.0);
                all_BlockAngle.push_back(AngleBlock - 360.0);
            } else {
                BlockAngle.push_back(AngleBlock);
                all_BlockAngle.push_back(AngleBlock);
            }

            if (BlockAngle.size() >= 2) {
				float mdiff = (BlockAngle.back() - BlockAngle[BlockAngle.size() - 2]);
				mdiff = mdiff >= 0 ? mdiff : mdiff + 360;

                for (int j = 0; j < 32; j++) {
                    double m_angle = (BlockAngle[BlockAngle.size() - 2] + (mdiff / 32) * j);
                    if (m_angle < 360.0) {
                        m_DataT[j].H_angle = m_angle;
                        m_DataT_d[j].H_angle = m_angle;
                    } else {
                        m_DataT[j].H_angle = m_angle - 360.0;
                        m_DataT_d[j].H_angle = m_angle - 360.0;
                    }


                    m_PointXYZ m_point = XYZ_calculate(m_DataT[j].ID, m_DataT[j].H_angle, m_DataT[j].Distance);
					m_DataT[j].X = m_point.x1;
					m_DataT[j].Y = m_point.y1;
					m_DataT[j].Z = m_point.z1;

                    m_PointXYZ m_point_d = XYZ_calculate(m_DataT_d[j].ID, m_DataT_d[j].H_angle, m_DataT_d[j].Distance);
                    m_DataT_d[j].X = m_point_d.x1;
                    m_DataT_d[j].Y = m_point_d.y1;
                    m_DataT_d[j].Z = m_point_d.z1;

                    LidarPointData mLidarPointData;

                    mLidarPointData.ID = m_DataT[j].ID;
                    mLidarPointData.X = m_point.x1;
                    mLidarPointData.Y = m_point.y1;
                    mLidarPointData.Z = m_point.z1;
                    mLidarPointData.Intensity = m_DataT[j].Intensity;

                    LidarPointData mLidarPointData_d;

                    mLidarPointData_d.ID = m_DataT_d[j].ID;
                    mLidarPointData_d.X = m_point_d.x1;
                    mLidarPointData_d.Y = m_point_d.y1;
                    mLidarPointData_d.Z = m_point_d.z1;
                    mLidarPointData_d.Intensity = m_DataT_d[j].Intensity;


                    if (all_BlockAngle.size() > 100) {
                        if ((m_DataT[j].H_angle - endFrameAngle > 0) && abs(m_DataT[j].H_angle - endFrameAngle) < 5) {
                            lidardata_tmp.push_back(mLidarPointData);
                            lidardata_tmp_d.push_back(mLidarPointData_d);
                            isOneFrame = true;
                        } else {
                            LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                            LidarPerFrameDatePrt_Get[mLidarPointData_d.ID].push_back(mLidarPointData_d);

                        }
                    } else {
                        LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                        LidarPerFrameDatePrt_Get[mLidarPointData_d.ID].push_back(mLidarPointData_d);
                    }
                }
            }

            for (int j = 0; j < 32; j++) {
                m_DataT[j].ID = j % 16;
                m_DataT[j].Distance =
                        (data[5 + 3 * j + 100 * i] * 256 + data[4 + 3 * j + 100 * i]) * IDistanceACC_C16_v3_0;
                m_DataT[j].Intensity = data[6 + 3 * j + 100 * i];
                m_DataT[j].Mtimestamp_nsce = lastAllTimestamp + (32 * (i / 2) + j + 1) * PointIntervalT_ns;

                m_DataT_d[j].ID = j % 16;
                m_DataT_d[j].Distance = (data[5 + 3 * j + 100 * (i + 1)] * 256 + data[4 + 3 * j + 100 * (i + 1)]) *
                                        IDistanceACC_C16_v3_0;
                m_DataT_d[j].Intensity = data[6 + 3 * j + 100 * (i + 1)];
                m_DataT_d[j].Mtimestamp_nsce = lastAllTimestamp + (32 * (i / 2) + j + 1) * PointIntervalT_ns;
            }


            if (i == 10) {
                float mdiff = (BlockAngle.back() - BlockAngle[BlockAngle.size() - 2]);
                mdiff = mdiff >= 0 ? mdiff : mdiff + 360;

                for (int j = 0; j < 32; j++) {
                    double m_angle = (BlockAngle.back() + (mdiff / 32) * j);
                    if (m_angle < 360.0)
                        m_DataT[j].H_angle = m_angle;
                    else
                        m_DataT[j].H_angle = m_angle - 360.0;

                    m_PointXYZ m_point_1 = XYZ_calculate(m_DataT[j].ID, m_DataT[j].H_angle, m_DataT[j].Distance);
                    m_DataT[j].X = m_point_1.x1;
                    m_DataT[j].Y = m_point_1.y1;
                    m_DataT[j].Z = m_point_1.z1;

                    m_PointXYZ m_point_d = XYZ_calculate(m_DataT_d[j].ID, m_DataT_d[j].H_angle, m_DataT_d[j].Distance);
                    m_DataT_d[j].X = m_point_d.x1;
                    m_DataT_d[j].Y = m_point_d.y1;
                    m_DataT_d[j].Z = m_point_d.z1;


                    LidarPointData mLidarPointData;

                    mLidarPointData.ID = m_DataT[j].ID;
                    mLidarPointData.X = m_point_1.x1;
                    mLidarPointData.Y = m_point_1.y1;
                    mLidarPointData.Z = m_point_1.z1;
                    mLidarPointData.Intensity = m_DataT[j].Intensity;

                    LidarPointData mLidarPointData_d;

                    mLidarPointData_d.ID = m_DataT_d[j].ID;
                    mLidarPointData_d.X = m_point_d.x1;
                    mLidarPointData_d.Y = m_point_d.y1;
                    mLidarPointData_d.Z = m_point_d.z1;
                    mLidarPointData_d.Intensity = m_DataT_d[j].Intensity;


                    if (all_BlockAngle.size() > 100) {
                        if ((m_DataT[j].H_angle - endFrameAngle > 0) && abs(m_DataT[j].H_angle - endFrameAngle) < 5) {
                            lidardata_tmp.push_back(mLidarPointData);
                            lidardata_tmp_d.push_back(mLidarPointData_d);
                            isOneFrame = true;
                        } else {
                            LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                            LidarPerFrameDatePrt_Get[mLidarPointData_d.ID].push_back(mLidarPointData_d);
                        }
                    } else {
                        LidarPerFrameDatePrt_Get[mLidarPointData.ID].push_back(mLidarPointData);
                        LidarPerFrameDatePrt_Get[mLidarPointData_d.ID].push_back(mLidarPointData_d);
                    }

                }
                BlockAngle.clear();
            }
        }

        if (isOneFrame) {

            sendLidarData();
            all_BlockAngle.clear();


            if (lidardata_tmp.size() > 0) {
                for (size_t SF = 0; SF < lidardata_tmp.size(); SF++) {
                    LidarPerFrameDatePrt_Get[lidardata_tmp[SF].ID].push_back(lidardata_tmp[SF]);
                    LidarPerFrameDatePrt_Get[lidardata_tmp_d[SF].ID].push_back(lidardata_tmp_d[SF]);
                }
            }
        }
    }
}

m_PointXYZ GetLidarData_C16_v3_0::XYZ_calculate(int ID, double H_angle, double Distance) {
    m_PointXYZ point;

    point.x1 = float(Distance * cosTheta[ID] * sin((H_angle) * PI / 180) +
                    ConversionDistance_C16_v3_0 * sin((H_angle - ConversionAngle_C16_v3_0) * PI / 180));

    point.y1 = float(Distance * cosTheta[ID] * cos(H_angle * PI / 180) +
                    ConversionDistance_C16_v3_0 * cos((H_angle - ConversionAngle_C16_v3_0) * PI / 180));

    point.z1 = float(Distance * sinTheta[ID]);

    return point;
}

#pragma region //设置雷达参数 

bool GetLidarData_C16_v3_0::setLidarRotateState(int RotateState) {
    if (setLidarParam()) {
        if (RotateState != 0 && RotateState != 1) {
            std::string str = "RotateState can only be equal to 0 or 1, please check the input parameters";
            messFunction(str, 0);
            return false;
        }
        //设置设备包端口
        Rest_UCWP_buff[40] = 0x00;
        Rest_UCWP_buff[41] = RotateState;
        return true;
    } else {
        std::string str = "Equipment package is not update!!!";
        messFunction(str, 0);
        return false;
    }
}


bool GetLidarData_C16_v3_0::setLidarWorkState(int LidarState) {
    if (setLidarParam()) {
        if (LidarState != 0 && LidarState != 1) {
            std::string str = "LidarState can only be equal to 0 or 1, please check the input parameters";
            messFunction(str, 0);
            return false;
        }
        Rest_UCWP_buff[44] = 0;
        Rest_UCWP_buff[45] = LidarState;
        return true;
    } else {
        std::string str = "Equipment package is not update!!!";
        messFunction(str, 0);
        return false;
    }
}

#pragma endregion 

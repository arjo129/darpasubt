#ifndef _ROS_vehicle_drive_UGV_Odom_h
#define _ROS_vehicle_drive_UGV_Odom_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace vehicle_drive
{

  class UGV_Odom : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef int32_t _encoderA_type;
      _encoderA_type encoderA;
      typedef int32_t _encoderB_type;
      _encoderB_type encoderB;
      typedef int32_t _encoderC_type;
      _encoderC_type encoderC;
      typedef int32_t _encoderD_type;
      _encoderD_type encoderD;
      typedef int32_t _wheelA_type;
      _wheelA_type wheelA;
      typedef int32_t _wheelB_type;
      _wheelB_type wheelB;
      typedef int32_t _wheelC_type;
      _wheelC_type wheelC;
      typedef int32_t _wheelD_type;
      _wheelD_type wheelD;

    UGV_Odom():
      header(),
      encoderA(0),
      encoderB(0),
      encoderC(0),
      encoderD(0),
      wheelA(0),
      wheelB(0),
      wheelC(0),
      wheelD(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderA;
      u_encoderA.real = this->encoderA;
      *(outbuffer + offset + 0) = (u_encoderA.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_encoderA.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_encoderA.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_encoderA.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->encoderA);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderB;
      u_encoderB.real = this->encoderB;
      *(outbuffer + offset + 0) = (u_encoderB.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_encoderB.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_encoderB.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_encoderB.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->encoderB);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderC;
      u_encoderC.real = this->encoderC;
      *(outbuffer + offset + 0) = (u_encoderC.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_encoderC.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_encoderC.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_encoderC.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->encoderC);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderD;
      u_encoderD.real = this->encoderD;
      *(outbuffer + offset + 0) = (u_encoderD.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_encoderD.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_encoderD.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_encoderD.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->encoderD);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelA;
      u_wheelA.real = this->wheelA;
      *(outbuffer + offset + 0) = (u_wheelA.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelA.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wheelA.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wheelA.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wheelA);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelB;
      u_wheelB.real = this->wheelB;
      *(outbuffer + offset + 0) = (u_wheelB.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelB.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wheelB.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wheelB.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wheelB);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelC;
      u_wheelC.real = this->wheelC;
      *(outbuffer + offset + 0) = (u_wheelC.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelC.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wheelC.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wheelC.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wheelC);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelD;
      u_wheelD.real = this->wheelD;
      *(outbuffer + offset + 0) = (u_wheelD.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelD.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wheelD.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wheelD.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wheelD);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderA;
      u_encoderA.base = 0;
      u_encoderA.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_encoderA.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_encoderA.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_encoderA.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->encoderA = u_encoderA.real;
      offset += sizeof(this->encoderA);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderB;
      u_encoderB.base = 0;
      u_encoderB.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_encoderB.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_encoderB.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_encoderB.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->encoderB = u_encoderB.real;
      offset += sizeof(this->encoderB);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderC;
      u_encoderC.base = 0;
      u_encoderC.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_encoderC.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_encoderC.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_encoderC.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->encoderC = u_encoderC.real;
      offset += sizeof(this->encoderC);
      union {
        int32_t real;
        uint32_t base;
      } u_encoderD;
      u_encoderD.base = 0;
      u_encoderD.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_encoderD.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_encoderD.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_encoderD.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->encoderD = u_encoderD.real;
      offset += sizeof(this->encoderD);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelA;
      u_wheelA.base = 0;
      u_wheelA.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelA.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wheelA.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wheelA.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wheelA = u_wheelA.real;
      offset += sizeof(this->wheelA);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelB;
      u_wheelB.base = 0;
      u_wheelB.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelB.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wheelB.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wheelB.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wheelB = u_wheelB.real;
      offset += sizeof(this->wheelB);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelC;
      u_wheelC.base = 0;
      u_wheelC.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelC.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wheelC.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wheelC.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wheelC = u_wheelC.real;
      offset += sizeof(this->wheelC);
      union {
        int32_t real;
        uint32_t base;
      } u_wheelD;
      u_wheelD.base = 0;
      u_wheelD.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelD.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wheelD.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wheelD.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wheelD = u_wheelD.real;
      offset += sizeof(this->wheelD);
     return offset;
    }

    const char * getType(){ return "vehicle_drive/UGV_Odom"; };
    const char * getMD5(){ return "e362e7411e14994120617cc636926ae5"; };

  };

}
#endif

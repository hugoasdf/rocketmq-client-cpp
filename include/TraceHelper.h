#ifndef __Tracehelper_H__
#define __Tracehelper_H__
#include <set>
#include <string>
#include <list>


namespace rocketmq {

//typedef int CommunicationMode;

enum MessageType {
  Normal_Msg,
  Trans_Msg_Half,
  Trans_msg_Commit,
  Delay_Msg,
};

enum TraceDispatcherType { PRODUCER, CONSUMER };

enum TraceType {
  Pub,
  SubBefore,
  SubAfter,
};


class TraceBean {
 public:
  static std::string LOCAL_ADDRESS;  //= "/*UtilAll.ipToIPv4Str(UtilAll.getIP())*/";
  
 private:
  std::string m_topic = "";
  std::string m_msgId = "";
  std::string m_offsetMsgId = "";
  std::string m_tags = "";
  std::string m_keys = "";
  std::string m_storeHost;  //  = LOCAL_ADDRESS;
  std::string m_clientHost;  //  = LOCAL_ADDRESS;
  long m_storeTime;
  int m_retryTimes;
  int m_bodyLength;
  MessageType m_msgType;

 public:
  MessageType getMsgType() { return m_msgType; };

  void setMsgType(MessageType msgTypev) { m_msgType = msgTypev; };

  std::string getOffsetMsgId() { return m_offsetMsgId; };

  void setOffsetMsgId(std::string offsetMsgIdv) { m_offsetMsgId = offsetMsgIdv; };

  std::string getTopic() { return m_topic; };

  void setTopic(std::string topicv) { m_topic = topicv; };

  std::string getMsgId() { return m_msgId; };

  void setMsgId(std::string msgIdv) { m_msgId = msgIdv; };

  std::string getTags() { return m_tags; };

  void setTags(std::string tagsv) { m_tags = tagsv; };

  std::string getKeys() { return m_keys; };

  void setKeys(std::string keysv) { m_keys = keysv; }

  std::string getStoreHost() { return m_storeHost; };

  void setStoreHost(std::string storeHostv) { m_storeHost = storeHostv; };

  std::string getClientHost() { return m_clientHost; };

  void setClientHost(std::string clientHostv) { m_clientHost = clientHostv; };

  long getStoreTime() { return m_storeTime; };

  void setStoreTime(long storeTimev) { m_storeTime = storeTimev; };

  int getRetryTimes() { return m_retryTimes; };

  void setRetryTimes(int retryTimesv) { m_retryTimes = retryTimesv; };

  int getBodyLength() { return m_bodyLength; };

  void setBodyLength(int bodyLengthv) { m_bodyLength = bodyLengthv; };
};

class TraceConstants {
 public:
  static std::string TraceConstants_GROUP_NAME;  // = "_INNER_TRACE_PRODUCER";
  static char CONTENT_SPLITOR;//  = (char)1;
  static char FIELD_SPLITOR;  // = (char)2;
  static std::string TRACE_INSTANCE_NAME;  // = "PID_CLIENT_INNER_TRACE_PRODUCER";
  static std::string TRACE_TOPIC_PREFIX;  // = MixAll.SYSTEM_TOPIC_PREFIX + "TRACE_DATA_";
};

class TraceContext /*: public Comparable<TraceContext>*/ {
 private:
  TraceType m_traceType;
  long m_timeStamp;  //= System.currentTimeMillis();
  std::string m_regionId = "";
  std::string m_regionName = "";
  std::string m_groupName = "";
  int m_costTime = 0;
  bool m_misSuccess = true;
  std::string m_requestId;  //= MessageClientIDSetter.createUniqID();
  int m_contextCode = 0;
  std::list<TraceBean> m_traceBeans;

 public:
  int getContextCode() { return m_contextCode; };

  void setContextCode(int contextCodev) { m_contextCode = contextCodev; };

  std::list<TraceBean>& getTraceBeans() { return m_traceBeans; };

  void setTraceBeans(std::list<TraceBean> traceBeansv) { m_traceBeans = traceBeansv; };

  std::string getRegionId() { return m_regionId; };

  void setRegionId(std::string regionIdv) { m_regionId = regionIdv; };

  TraceType getTraceType() { return m_traceType; };

  void setTraceType(TraceType traceTypev) { m_traceType = traceTypev; };

  long getTimeStamp() { return m_timeStamp; };

  void setTimeStamp(long timeStampv) { m_timeStamp = timeStampv; };

  std::string getGroupName() { return m_groupName; };

  void setGroupName(std::string groupNamev) { m_groupName = groupNamev; };

  int getCostTime() { return m_costTime; };

  void setCostTime(int costTimev) { m_costTime = costTimev; };

  bool isSuccess() { return m_misSuccess; };

  void setSuccess(bool successv) { m_misSuccess = successv; };

  std::string getRequestId() { return m_requestId; };

  void setRequestId(std::string requestIdv) { m_requestId = requestIdv; };

  std::string getRegionName() { return m_regionName; };

  void setRegionName(std::string regionName) { m_regionName = regionName; };

  int compareTo(TraceContext o) { return (int)(m_timeStamp - o.getTimeStamp());
  };

  std::string tostring() {
    std::string sb;//    = new std::stringBuilder(1024);
    /*sb.append( ((char)traceType) )
        .append("_")
        .append(groupName)
        .append("_")
        .append(regionId)
        .append("_")
        .append(misSuccess)
        .append("_");*/
    if (/*traceBeans != nullptr &&*/ m_traceBeans.size() > 0) {
      for (TraceBean bean : m_traceBeans) {
        sb.append(bean.getMsgId() + "_" + bean.getTopic() + "_");
      }
    }
    return "TraceContext{" + sb + '}';
  }
};



class TraceTransferBean {
 private:
  std::string m_transData;
  std::set<std::string> m_transKey;  //  = new std::set<std::string>();

 public:
  std::string getTransData() { return m_transData; }

  void setTransData(const std::string& transDatav) { m_transData = transDatav; }

  std::set<std::string> getTransKey() { return m_transKey; }

  void setTransKey(std::set<std::string> transKeyv) { m_transKey = transKeyv; }
};







class TraceDataEncoder{
public:
	static std::list<TraceContext> decoderFromTraceDatastring(const std::string& traceData);
	static TraceTransferBean encoderFromContextBean(TraceContext* ctx);

};





}  // namespace rocketmq



#endif
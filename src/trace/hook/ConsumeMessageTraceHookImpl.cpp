/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ConsumeMessageTraceHookImpl.h"
#include "MQMessageExt.h"

namespace rocketmq {

		ConsumeMessageTraceHookImpl::ConsumeMessageTraceHookImpl(std::shared_ptr<TraceDispatcher>& localDispatcherv) {
  localDispatcher = std::shared_ptr<TraceDispatcher>(localDispatcherv);
		}

		std::string ConsumeMessageTraceHookImpl::hookName() {
			return "ConsumeMessageTraceHook";
		}

		void ConsumeMessageTraceHookImpl::consumeMessageBefore(ConsumeMessageContext* context) {
			if (context == nullptr || context->getMsgList().empty()) {
				return;
			}
			TraceContext traceContext;
			context->setMqTraceContext(&traceContext);
			traceContext.setTraceType(TraceType::SubBefore);//
			traceContext.setGroupName("NamespaceUtil.withoutNamespace(context.getConsumerGroup())");//
			std::list<TraceBean> beans;
			for (MQMessageExt& msg : context->getMsgList()) {
				/*if (msg == nullptr) {
					continue;
				}*/
				std::string regionId = msg.getProperty(MQMessage::PROPERTY_MSG_REGION);
        std::string traceOn = msg.getProperty(MQMessage::PROPERTY_TRACE_SWITCH);

				if (traceOn ==("false")) {
					// If trace switch is false ,skip it
					continue;
				}
				TraceBean traceBean;
				traceBean.setTopic("NamespaceUtil.withoutNamespace(msg.getTopic())");//
				traceBean.setMsgId(msg.getMsgId());//
				traceBean.setTags(msg.getTags());//
				traceBean.setKeys(msg.getKeys());//
				traceBean.setStoreTime(msg.getStoreTimestamp());//
				traceBean.setBodyLength(msg.getStoreSize());//
				traceBean.setRetryTimes(msg.getReconsumeTimes());//
				traceContext.setRegionId(regionId);//
				beans.push_back(traceBean);
			}
			if (beans.size() > 0) {
				traceContext.setTraceBeans(beans);
        traceContext.setTimeStamp(time(0));
				localDispatcher->append(&traceContext);
			}
		}

		void ConsumeMessageTraceHookImpl::consumeMessageAfter(ConsumeMessageContext* context) {
			if (context == nullptr || context->getMsgList().empty()) {
				return;
			}
			TraceContext* subBeforeContext = (TraceContext*)context->getMqTraceContext();

			if (subBeforeContext->getTraceBeans().empty() || subBeforeContext->getTraceBeans().size() < 1) {
				// If subbefore bean is nullptr ,skip it
				return;
			}
			TraceContext subAfterContext;
			subAfterContext.setTraceType(TraceType::SubAfter);//
			subAfterContext.setRegionId(subBeforeContext->getRegionId());//
			subAfterContext.setGroupName("NamespaceUtil.withoutNamespace(subBeforeContext.getGroupName())");//
			subAfterContext.setRequestId(subBeforeContext->getRequestId());//
      subAfterContext.setSuccess(context ->isSuccess());                                              //

			// Caculate the cost time for processing messages
			int costTime = 0;//(int)((System.currentTimeMillis() - subBeforeContext.getTimeStamp()) / context.getMsgList().size());
			subAfterContext.setCostTime(costTime);//
			subAfterContext.setTraceBeans(subBeforeContext->getTraceBeans());
			std::string contextType = context->getProps().at("MixAll::CONSUME_CONTEXT_TYPE");
			if (contextType.empty()) {
				//subAfterContext.setContextCode(ConsumeReturnType.valueOf(contextType).ordinal());
			}
			localDispatcher->append(&subAfterContext);
		}










}//ns

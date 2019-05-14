bin_PROGRAMS=publisher

publisher_SOURCES= main.cpp 

publisher_LDADD= -L/home/ceneblock/OpenDDS/ACE_TAO/ACE/lib -L/home/ceneblock/OpenDDS/lib -L.  -L/home/ceneblock/OpenDDS/ACE_TAO/ACE/lib  -L../../../lib
publisher_LDFLAGS=-fvisibility=hidden -fvisibility-inlines-hidden -Wnon-virtual-dtor -ggdb -pthread -fno-strict-aliasing -Wall -W -Wpointer-arith -pipe -D_GNU_SOURCE   -I/home/ceneblock/OpenDDS/ACE_TAO/ACE -D__ACE_INLINE__ -I/home/ceneblock/OpenDDS/ACE_TAO/ACE -I/home/ceneblock/OpenDDS/ACE_TAO/TAO -lOpenDDS_Tcp -lOpenDDS_InfoRepoDiscovery -lOpenDDS_Dcps -lTAO_BiDirGIOP -lTAO_PI -lTAO_CodecFactory -lTAO_PortableServer -lTAO_AnyTypeCode -lTAO -lACE -ldl -lrt

main.cpp: MessengerTypeSupportC.cpp MessengerTypeSupportS.cpp MessengerC.cpp MessengerS.cpp MessengerTypeSupportImpl.cpp

clean-local:
	-rm MessengerTypeSupportImpl.cpp MessengerTypeSupportImpl.h
	-rm MessengerTypeSupportC.cpp MessengerTypeSupportC.h
	-rm MessengerTypeSupportS.cpp MessengerTypeSupportS.h
	-rm	MessengerC.cpp MessengerC.h 
	-rm MessengerS.cpp MessengerS.h
	-rm MessengerTypeSupport.idl MessengerTypeSupportC.inl MessengerC.inl
	-rm publisher

MessengerTypeSupportImpl.cpp: Messenger.idl
#opendds_idl -Wb,export_macro=MessengerCommon_Export -Wb,export_include=MessengerCommon_Export.h --export=MessengerCommon_Export Messenger.idl 
	opendds_idl Messenger.idl 

MessengerTypeSupportC.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl -I$(DDS_ROOT) MessengerTypeSupport.idl

MessengerTypeSupportS.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl -I$(DDS_ROOT) MessengerTypeSupport.idl

MessengerC.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl -I$(DDS_ROOT) MessengerTypeSupport.idl

MessengerS.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl -I$(DDS_ROOT) MessengerTypeSupport.idl

#Making a fake main.cpp target so we get all of our idl files created
.PHONY:
	main.cpp clean-local



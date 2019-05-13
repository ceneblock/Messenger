bin_PROGRAMS=messenger

messenger_SOURCES= main.cpp 


main.cpp: MessengerTypeSupportC.cpp MessengerTypeSupportS.cpp MessengerC.cpp MessengerS.cpp MessengerTypeSupportImpl.cpp

clean-local:
	-rm MessengerTypeSupportImpl.cpp MessengerTypeSupportImpl.h
	-rm MessengerTypeSupportC.cpp MessengerTypeSupportC.h
	-rm MessengerTypeSupportS.cpp MessengerTypeSupportS.h
	-rm	MessengerC.cpp MessengerC.h 
	-rm MessengerS.cpp MessengerS.h
	-rm MessengerTypeSupport.idl MessengerTypeSupportC.inl MessengerC.inl


MessengerTypeSupportImpl.cpp: Messenger.idl
#opendds_idl -Wb,export_macro=MessengerCommon_Export -Wb,export_include=MessengerCommon_Export.h --export=MessengerCommon_Export Messenger.idl 
	opendds_idl Messenger.idl 

MessengerTypeSupportC.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl MessengerTypeSupport.idl

MessengerTypeSupportS.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl MessengerTypeSupport.idl

MessengerC.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl MessengerTypeSupport.idl

MessengerS.cpp: MessengerTypeSupportImpl.cpp
	tao_idl Messenger.idl 
	tao_idl MessengerTypeSupport.idl

#Making a fake main.cpp target so we get all of our idl files created
.PHONY:
	main.cpp clean-local



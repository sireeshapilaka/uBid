#
# OMNeT++/OMNEST Makefile for uBid
#
# This file was generated with the command:
#  opp_makemake -f --deep -I/Library/gurobi751/mac64/include -I/Users/sireeshapilaka/inet/src -L/Users/sireeshapilaka/inet/out/$$\(CONFIGNAME\)/src -lINET -KGUROBI_HOME=/Library/gurobi751/mac64 -KGRB_LICENSE_FILE=/Users/sireeshapilaka/gurobi.lic -KINET_PROJ=/Users/sireeshapilaka/inet
#

# Name of target to be created (-o option)
TARGET = uBid$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/Library/gurobi751/mac64/include \
    -I$(INET_PROJ)/src \
    -I. \
    -Icorenetwork \
    -Icorenetwork/Negotiation \
    -Icorenetwork/nodes \
    -Idao

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(INET_PROJ)/out/$(CONFIGNAME)/src  -lINET
LIBS += -Wl,-rpath,`abspath $(INET_PROJ)/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = $O/corenetwork/Negotiation/NetworkAgent.o $O/corenetwork/Negotiation/ResourceManager.o $O/corenetwork/nodes/Ue.o $O/corenetwork/nodes/UserAgent.o $O/dao/BidResponse.o

# Message files
MSGFILES =

# SM files
SMFILES =

# Other makefile variables (-K)
GUROBI_HOME=/Library/gurobi751/mac64
GRB_LICENSE_FILE=/Users/sireeshapilaka/gurobi.lic
INET_PROJ=/Users/sireeshapilaka/inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
EXTRA_OBJS += -L/Library/gurobi751/mac64/lib -lgurobi_c++
CFLAGS += -I/Library/gurobi751/mac64/inlcude

# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f uBid uBid.exe libuBid.so libuBid.a libuBid.dll libuBid.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f corenetwork/*_m.cc corenetwork/*_m.h corenetwork/*_sm.cc corenetwork/*_sm.h
	$(Q)-rm -f corenetwork/Negotiation/*_m.cc corenetwork/Negotiation/*_m.h corenetwork/Negotiation/*_sm.cc corenetwork/Negotiation/*_sm.h
	$(Q)-rm -f corenetwork/nodes/*_m.cc corenetwork/nodes/*_m.h corenetwork/nodes/*_sm.cc corenetwork/nodes/*_sm.h
	$(Q)-rm -f dao/*_m.cc dao/*_m.h dao/*_sm.cc dao/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES) $(SM_CC_FILES)  ./*.cc corenetwork/*.cc corenetwork/Negotiation/*.cc corenetwork/nodes/*.cc dao/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/corenetwork/Negotiation/NetworkAgent.o: corenetwork/Negotiation/NetworkAgent.cc \
	$(INET_PROJ)/src/inet/applications/base/ApplicationBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowser.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowserBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpEventMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpNodeBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpRandom.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpUtils.h \
	$(INET_PROJ)/src/inet/applications/httptools/configurator/HttpController.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServer.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServerBase.h \
	$(INET_PROJ)/src/inet/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamCli.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamSvr.h \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3AddressResolver.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IIPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPCommand_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocket.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocketMap.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPSocket.h \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	dao/BidResponse.h
$O/corenetwork/Negotiation/ResourceManager.o: corenetwork/Negotiation/ResourceManager.cc \
	corenetwork/Negotiation/ResourceManager.h
$O/corenetwork/nodes/Ue.o: corenetwork/nodes/Ue.cc \
	$(INET_PROJ)/src/inet/applications/base/ApplicationBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowser.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowserBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpEventMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpNodeBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpRandom.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpUtils.h \
	$(INET_PROJ)/src/inet/applications/httptools/configurator/HttpController.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServer.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServerBase.h \
	$(INET_PROJ)/src/inet/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamCli.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamSvr.h \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3AddressResolver.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IIPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPCommand_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocket.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocketMap.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPSocket.h \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UserAgent.h \
	dao/BidResponse.h
$O/corenetwork/nodes/UserAgent.o: corenetwork/nodes/UserAgent.cc \
	$(INET_PROJ)/src/inet/applications/base/ApplicationBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowser.h \
	$(INET_PROJ)/src/inet/applications/httptools/browser/HttpBrowserBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpEventMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpMessages_m.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpNodeBase.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpRandom.h \
	$(INET_PROJ)/src/inet/applications/httptools/common/HttpUtils.h \
	$(INET_PROJ)/src/inet/applications/httptools/configurator/HttpController.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServer.h \
	$(INET_PROJ)/src/inet/applications/httptools/server/HttpServerBase.h \
	$(INET_PROJ)/src/inet/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamCli.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPVideoStreamSvr.h \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3AddressResolver.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IIPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPCommand_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocket.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/tcp/TCPSocketMap.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPSocket.h \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UserAgent.h \
	dao/BidResponse.h
$O/dao/BidResponse.o: dao/BidResponse.cc \
	dao/BidResponse.h


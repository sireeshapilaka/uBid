#
# OMNeT++/OMNEST Makefile for uBid1
#
# This file was generated with the command:
#  opp_makemake -f --deep -I/Library/gurobi751/mac64/include -KGUROBI_HOME=/Library/gurobi751/mac64 -KGRB_LICENSE_FILE=/Users/sireeshapilaka/gurobi.lic
#

# Name of target to be created (-o option)
TARGET = uBid1$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/Library/gurobi751/mac64/include \
    -I. \
    -Icorenetwork \
    -Icorenetwork/Negotiation \
    -Icorenetwork/nodes \
    -Idao \
    -Iutil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/corenetwork/Negotiation/NetworkAgent.o \
    $O/corenetwork/Negotiation/ResourceManager.o \
    $O/corenetwork/nodes/Ue.o \
    $O/corenetwork/nodes/UeMC.o \
    $O/corenetwork/nodes/UserAgentMC.o \
    $O/corenetwork/nodes/UserAgent.o \
    $O/dao/AppBwRes.o \
    $O/dao/BidResponse.o \
    $O/dao/ActivityDAO.o \
    $O/dao/AppBwReq.o \
    $O/dao/AppAccessResponse.o \
    $O/util/UaUtils.o \
    $O/util/StrUtils.o \
    $O/util/AppTrafficParser.o

# Message files
MSGFILES =

# SM files
SMFILES =

# Other makefile variables (-K)
GUROBI_HOME=/Library/gurobi751/mac64
GRB_LICENSE_FILE=/Users/sireeshapilaka/gurobi.lic

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
CMAKE_CXX_FLAGS += -stdlib=libstdc++
EXTRA_OBJS += -L/Library/gurobi751/mac64/lib -lgurobi_c++ -lgurobi75
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
	$(Q)-rm -f uBid1 uBid1.exe libuBid1.so libuBid1.a libuBid1.dll libuBid1.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f corenetwork/*_m.cc corenetwork/*_m.h corenetwork/*_sm.cc corenetwork/*_sm.h
	$(Q)-rm -f corenetwork/Negotiation/*_m.cc corenetwork/Negotiation/*_m.h corenetwork/Negotiation/*_sm.cc corenetwork/Negotiation/*_sm.h
	$(Q)-rm -f corenetwork/nodes/*_m.cc corenetwork/nodes/*_m.h corenetwork/nodes/*_sm.cc corenetwork/nodes/*_sm.h
	$(Q)-rm -f dao/*_m.cc dao/*_m.h dao/*_sm.cc dao/*_sm.h
	$(Q)-rm -f util/*_m.cc util/*_m.h util/*_sm.cc util/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES) $(SM_CC_FILES)  ./*.cc corenetwork/*.cc corenetwork/Negotiation/*.cc corenetwork/nodes/*.cc dao/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/corenetwork/Negotiation/NetworkAgent.o: corenetwork/Negotiation/NetworkAgent.cc \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	dao/ActivityDAO.h \
	dao/AppAccessResponse.h \
	dao/AppBwReq.h \
	dao/AppBwRes.h \
	dao/BidResponse.h \
	util/ModeConstants.h \
	util/StrUtils.h
$O/corenetwork/Negotiation/ResourceManager.o: corenetwork/Negotiation/ResourceManager.cc \
	corenetwork/Negotiation/ResourceManager.h \
	util/ModeConstants.h
$O/corenetwork/nodes/Ue.o: corenetwork/nodes/Ue.cc \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UserAgent.h \
	dao/ActivityDAO.h \
	dao/AppAccessResponse.h \
	dao/AppBwReq.h \
	dao/AppBwRes.h \
	dao/BidResponse.h \
	util/AppTrafficParser.h \
	util/ModeConstants.h \
	util/StrUtils.h \
	util/UaUtils.h
$O/corenetwork/nodes/UeMC.o: corenetwork/nodes/UeMC.cc \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UeMC.h \
	corenetwork/nodes/UserAgent.h \
	corenetwork/nodes/UserAgentMC.h \
	dao/ActivityDAO.h \
	dao/AppAccessResponse.h \
	dao/AppBwReq.h \
	dao/AppBwRes.h \
	dao/BidResponse.h \
	util/ModeConstants.h \
	util/StrUtils.h \
	util/UaUtils.h
$O/corenetwork/nodes/UserAgent.o: corenetwork/nodes/UserAgent.cc \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UserAgent.h \
	dao/ActivityDAO.h \
	dao/AppAccessResponse.h \
	dao/AppBwReq.h \
	dao/AppBwRes.h \
	dao/BidResponse.h \
	util/ModeConstants.h \
	util/StrUtils.h \
	util/UaUtils.h
$O/corenetwork/nodes/UserAgentMC.o: corenetwork/nodes/UserAgentMC.cc \
	corenetwork/Negotiation/NetworkAgent.h \
	corenetwork/Negotiation/ResourceManager.h \
	corenetwork/nodes/Ue.h \
	corenetwork/nodes/UserAgent.h \
	corenetwork/nodes/UserAgentMC.h \
	dao/ActivityDAO.h \
	dao/AppAccessResponse.h \
	dao/AppBwReq.h \
	dao/AppBwRes.h \
	dao/BidResponse.h \
	util/ModeConstants.h \
	util/StrUtils.h \
	util/UaUtils.h
$O/dao/ActivityDAO.o: dao/ActivityDAO.cc \
	dao/ActivityDAO.h
$O/dao/AppAccessResponse.o: dao/AppAccessResponse.cc \
	dao/AppAccessResponse.h
$O/dao/AppBwReq.o: dao/AppBwReq.cc \
	dao/AppBwReq.h
$O/dao/AppBwRes.o: dao/AppBwRes.cc \
	dao/AppBwRes.h
$O/dao/BidResponse.o: dao/BidResponse.cc \
	dao/BidResponse.h
$O/util/AppTrafficParser.o: util/AppTrafficParser.cc \
	dao/ActivityDAO.h \
	util/AppTrafficParser.h
$O/util/StrUtils.o: util/StrUtils.cc \
	util/StrUtils.h
$O/util/UaUtils.o: util/UaUtils.cc \
	util/ModeConstants.h \
	util/UaUtils.h


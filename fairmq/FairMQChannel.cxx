/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQChannel.cxx
 *
 * @since 2015-06-02
 * @author A. Rybalchenko
 */

#include <set>

#include <boost/exception/all.hpp>

#include "FairMQChannel.h"
#include "FairMQLogger.h"

using namespace std;

boost::mutex FairMQChannel::fChannelMutex;

FairMQChannel::FairMQChannel()
    : fSocket(nullptr)
    , fType("unspecified")
    , fMethod("unspecified")
    , fAddress("unspecified")
    , fProperty("")
    , fSndBufSize(1000)
    , fRcvBufSize(1000)
    , fSndKernelSize(0)
    , fRcvKernelSize(0)
    , fRateLogging(1)
    , fChannelName("")
    , fIsValid(false)
    , fPoller(nullptr)
    , fCmdSocket(nullptr)
    , fTransportFactory(nullptr)
    , fNoBlockFlag(0)
    , fSndMoreFlag(0)
    , fSndTimeoutInMs(-1)
    , fRcvTimeoutInMs(-1)
{
}

FairMQChannel::FairMQChannel(const string& type, const string& method, const string& address)
    : fSocket(nullptr)
    , fType(type)
    , fMethod(method)
    , fAddress(address)
    , fProperty("")
    , fSndBufSize(1000)
    , fRcvBufSize(1000)
    , fSndKernelSize(0)
    , fRcvKernelSize(0)
    , fRateLogging(1)
    , fChannelName("")
    , fIsValid(false)
    , fPoller(nullptr)
    , fCmdSocket(nullptr)
    , fTransportFactory(nullptr)
    , fNoBlockFlag(0)
    , fSndMoreFlag(0)
    , fSndTimeoutInMs(-1)
    , fRcvTimeoutInMs(-1)
{
}

FairMQChannel::FairMQChannel(const FairMQChannel& chan)
    : fSocket(nullptr)
    , fType(chan.fType)
    , fMethod(chan.fMethod)
    , fAddress(chan.fAddress)
    , fProperty(chan.fProperty)
    , fSndBufSize(chan.fSndBufSize)
    , fRcvBufSize(chan.fRcvBufSize)
    , fSndKernelSize(chan.fSndKernelSize)
    , fRcvKernelSize(chan.fRcvKernelSize)
    , fRateLogging(chan.fRateLogging)
    , fChannelName(chan.fChannelName)
    , fIsValid(false)
    , fPoller(nullptr)
    , fCmdSocket(nullptr)
    , fTransportFactory(nullptr)
    , fNoBlockFlag(chan.fNoBlockFlag)
    , fSndMoreFlag(chan.fSndMoreFlag)
    , fSndTimeoutInMs(chan.fSndTimeoutInMs)
    , fRcvTimeoutInMs(chan.fRcvTimeoutInMs)
{}

FairMQChannel& FairMQChannel::operator=(const FairMQChannel& chan)
{
    fType = chan.fType;
    fMethod = chan.fMethod;
    fAddress = chan.fAddress;
    fProperty = chan.fProperty;
    fSndBufSize = chan.fSndBufSize;
    fRcvBufSize = chan.fRcvBufSize;
    fSndKernelSize = chan.fSndKernelSize;
    fRcvKernelSize = chan.fRcvKernelSize;
    fRateLogging = chan.fRateLogging;
    fSocket = nullptr;
    fChannelName = chan.fChannelName;
    fIsValid = false;
    fPoller = nullptr;
    fCmdSocket = nullptr;
    fTransportFactory = nullptr;
    fNoBlockFlag = chan.fNoBlockFlag;
    fSndMoreFlag = chan.fSndMoreFlag;
    fSndTimeoutInMs = chan.fSndTimeoutInMs;
    fRcvTimeoutInMs = chan.fRcvTimeoutInMs;

    return *this;
}

string FairMQChannel::GetChannelName() const
{
    return fChannelName;
}

string FairMQChannel::GetChannelPrefix() const
{
    string prefix = fChannelName;
    return prefix.erase(fChannelName.rfind("["));
}

string FairMQChannel::GetType() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fType;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetType: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

string FairMQChannel::GetMethod() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fMethod;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetMethod: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

string FairMQChannel::GetAddress() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fAddress;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetAddress: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

string FairMQChannel::GetProperty() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fProperty;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetProperty: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

int FairMQChannel::GetSndBufSize() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fSndBufSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetSndBufSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

int FairMQChannel::GetRcvBufSize() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fRcvBufSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetRcvBufSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

int FairMQChannel::GetSndKernelSize() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fSndKernelSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetSndKernelSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

int FairMQChannel::GetRcvKernelSize() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fRcvKernelSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetRcvKernelSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

int FairMQChannel::GetRateLogging() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fRateLogging;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::GetRateLogging: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateType(const string& type)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fType = type;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateType: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateMethod(const string& method)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fMethod = method;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateMethod: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateAddress(const string& address)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fAddress = address;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateAddress: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateProperty(const string& property)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fProperty = property;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateProperty: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateSndBufSize(const int sndBufSize)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fSndBufSize = sndBufSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateSndBufSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateRcvBufSize(const int rcvBufSize)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fRcvBufSize = rcvBufSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateRcvBufSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateSndKernelSize(const int sndKernelSize)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fSndKernelSize = sndKernelSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateSndKernelSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateRcvKernelSize(const int rcvKernelSize)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fRcvKernelSize = rcvKernelSize;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateRcvKernelSize: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

void FairMQChannel::UpdateRateLogging(const int rateLogging)
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        fIsValid = false;
        fRateLogging = rateLogging;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::UpdateRateLogging: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

bool FairMQChannel::IsValid() const
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);
        return fIsValid;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::IsValid: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

bool FairMQChannel::ValidateChannel()
{
    try
    {
        boost::unique_lock<boost::mutex> scoped_lock(fChannelMutex);

        stringstream ss;
        ss << "Validating channel \"" << fChannelName << "\"... ";

        if (fIsValid)
        {
            ss << "ALREADY VALID";
            LOG(DEBUG) << ss.str();
            return true;
        }

        // validate socket type
        const string socketTypeNames[] = { "sub", "pub", "pull", "push", "req", "rep", "xsub", "xpub", "dealer", "router", "pair" };
        const set<string> socketTypes(socketTypeNames, socketTypeNames + sizeof(socketTypeNames) / sizeof(string));
        if (socketTypes.find(fType) == socketTypes.end())
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "Invalid channel type: \"" << fType << "\"";
            return false;
        }

        // validate socket method
        const string socketMethodNames[] = { "bind", "connect" };
        const set<string> socketMethods(socketMethodNames, socketMethodNames + sizeof(socketMethodNames) / sizeof(string));
        if (socketMethods.find(fMethod) == socketMethods.end())
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "Invalid channel method: \"" << fMethod << "\"";
            return false;
        }

        // validate socket address
        if (fAddress == "unspecified" || fAddress == "")
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "invalid channel address: \"" << fAddress << "\"";
            return false;
        }
        else
        {
            // check if address is a tcp or ipc address
            if (fAddress.compare(0, 6, "tcp://") == 0)
            {
                // check if TCP address contains port delimiter
                string addressString = fAddress.substr(6);
                if (addressString.find(":") == string::npos)
                {
                    ss << "INVALID";
                    LOG(DEBUG) << ss.str();
                    LOG(DEBUG) << "invalid channel address: \"" << fAddress << "\" (missing port?)";
                    return false;
                }
            }
            else if (fAddress.compare(0, 6, "ipc://") == 0)
            {
                // check if IPC address is not empty
                string addressString = fAddress.substr(6);
                if (addressString == "")
                {
                    ss << "INVALID";
                    LOG(DEBUG) << ss.str();
                    LOG(DEBUG) << "invalid channel address: \"" << fAddress << "\" (empty IPC address?)";
                    return false;
                }
            }
            else
            {
                // if neither TCP or IPC is specified, return invalid
                ss << "INVALID";
                LOG(DEBUG) << ss.str();
                LOG(DEBUG) << "invalid channel address: \"" << fAddress << "\" (missing protocol specifier?)";
                return false;
            }
        }

        // validate socket buffer size for sending
        if (fSndBufSize < 0)
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "invalid channel send buffer size: \"" << fSndBufSize << "\"";
            return false;
        }

        // validate socket buffer size for receiving
        if (fRcvBufSize < 0)
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "invalid channel receive buffer size: \"" << fRcvBufSize << "\"";
            return false;
        }

        // validate socket kernel transmit size for sending
        if (fSndKernelSize < 0)
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "invalid channel send kernel transmit size: \"" << fSndKernelSize << "\"";
            return false;
        }

        // validate socket kernel transmit size for receiving
        if (fRcvKernelSize < 0)
        {
            ss << "INVALID";
            LOG(DEBUG) << ss.str();
            LOG(DEBUG) << "invalid channel receive kernel transmit size: \"" << fRcvKernelSize << "\"";
            return false;
        }

        fIsValid = true;
        ss << "VALID";
        LOG(DEBUG) << ss.str();
        return true;
    }
    catch (boost::exception& e)
    {
        LOG(ERROR) << "Exception caught in FairMQChannel::ValidateChannel: " << boost::diagnostic_information(e);
        exit(EXIT_FAILURE);
    }
}

bool FairMQChannel::InitCommandInterface(FairMQTransportFactory* factory, int numIoThreads)
{
    fTransportFactory = factory;

    fCmdSocket = fTransportFactory->CreateSocket("sub", "device-commands", numIoThreads, "internal");
    if (fCmdSocket)
    {
        fCmdSocket->Connect("inproc://commands");

        fNoBlockFlag = fCmdSocket->NOBLOCK;
        fSndMoreFlag = fCmdSocket->SNDMORE;

        fPoller = fTransportFactory->CreatePoller(*fCmdSocket, *fSocket);

        return true;
    }
    else
    {
        return false;
    }
}

void FairMQChannel::ResetChannel()
{
    fIsValid = false;
    // TODO: implement channel resetting
}

int FairMQChannel::Send(const unique_ptr<FairMQMessage>& msg) const
{
    fPoller->Poll(fSndTimeoutInMs);

    if (fPoller->CheckInput(0))
    {
        HandleUnblock();
        return -2;
    }

    if (fPoller->CheckOutput(1))
    {
        return fSocket->Send(msg.get(), 0);
    }

    return -2;
}

int FairMQChannel::Receive(const unique_ptr<FairMQMessage>& msg) const
{
    fPoller->Poll(fRcvTimeoutInMs);

    if (fPoller->CheckInput(0))
    {
        HandleUnblock();
        return -2;
    }

    if (fPoller->CheckInput(1))
    {
        return fSocket->Receive(msg.get(), 0);
    }

    return -2;
}

int64_t FairMQChannel::Send(const std::vector<std::unique_ptr<FairMQMessage>>& msgVec) const
{
    fPoller->Poll(fSndTimeoutInMs);

    if (fPoller->CheckInput(0))
    {
        HandleUnblock();
        return -2;
    }

    if (fPoller->CheckOutput(1))
    {
        return fSocket->Send(msgVec);
    }

    return -2;
}

int64_t FairMQChannel::Receive(std::vector<std::unique_ptr<FairMQMessage>>& msgVec) const
{
    fPoller->Poll(fRcvTimeoutInMs);

    if (fPoller->CheckInput(0))
    {
        HandleUnblock();
        return -2;
    }

    if (fPoller->CheckInput(1))
    {
        return fSocket->Receive(msgVec);
    }

    return -2;
}

int FairMQChannel::Send(FairMQMessage* msg, const string& flag) const
{
    if (flag == "")
    {
        fPoller->Poll(fSndTimeoutInMs);

        if (fPoller->CheckInput(0))
        {
            HandleUnblock();
            return -2;
        }

        if (fPoller->CheckOutput(1))
        {
            return fSocket->Send(msg, flag);
        }

        return -2;
    }
    else
    {
        return fSocket->Send(msg, flag);
    }
}

int FairMQChannel::Send(FairMQMessage* msg, const int flags) const
{
    if (flags == 0)
    {
        fPoller->Poll(fSndTimeoutInMs);

        if (fPoller->CheckInput(0))
        {
            HandleUnblock();
            return -2;
        }

        if (fPoller->CheckOutput(1))
        {
            return fSocket->Send(msg, flags);
        }

        return -2;
    }
    else
    {
        return fSocket->Send(msg, flags);
    }
}

int FairMQChannel::Receive(FairMQMessage* msg, const string& flag) const
{
    if (flag == "")
    {
        fPoller->Poll(fRcvTimeoutInMs);

        if (fPoller->CheckInput(0))
        {
            HandleUnblock();
            return -2;
        }

        if (fPoller->CheckInput(1))
        {
            return fSocket->Receive(msg, flag);
        }

        return -2;
    }
    else
    {
        return fSocket->Receive(msg, flag);
    }
}

int FairMQChannel::Receive(FairMQMessage* msg, const int flags) const
{
    if (flags == 0)
    {
        fPoller->Poll(fRcvTimeoutInMs);

        if (fPoller->CheckInput(0))
        {
            HandleUnblock();
            return -2;
        }

        if (fPoller->CheckInput(1))
        {
            return fSocket->Receive(msg, flags);
        }

        return -2;
    }
    else
    {
        return fSocket->Receive(msg, flags);
    }
}

bool FairMQChannel::ExpectsAnotherPart() const
{
    int64_t more = 0;
    size_t more_size = sizeof more;

    if (fSocket)
    {
        fSocket->GetOption("rcv-more", &more, &more_size);
        if (more)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

inline bool FairMQChannel::HandleUnblock() const
{
    FairMQMessage* cmd = fTransportFactory->CreateMessage();
    if (fCmdSocket->Receive(cmd, 0) >= 0)
    {
        LOG(DEBUG) << "unblocked";
    }
    delete cmd;
    return true;
}

FairMQChannel::~FairMQChannel()
{
    delete fCmdSocket;
    delete fPoller;
}

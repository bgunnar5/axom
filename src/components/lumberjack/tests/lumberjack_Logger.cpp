/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

#include "gtest/gtest.h"

#include "lumberjack/Logger.hpp"

#include "common/CommonTypes.hpp"
#include "lumberjack/Communicator.hpp"
#include "lumberjack/Message.hpp"

#include <stdlib.h>
#include <time.h>

class TestCommunicator: public asctoolkit::lumberjack::Communicator {
    public:
        void initialize(MPI_Comm comm, int ranksLimit)
        {
        	m_mpiComm = comm;
        	m_ranksLimit = ranksLimit;
        	srand(time(ATK_NULLPTR));
        }

        void finalize()
        {

        }

        void pushMessagesOnce(std::vector<asctoolkit::lumberjack::Message*>& messages)
        {
        	//Quiets a warning
        	messages = messages;
        }

        void pushMessagesFully(std::vector<asctoolkit::lumberjack::Message*>& messages)
        {
        	//Quiets a warning
        	messages = messages;
        }

        bool shouldMessagesBeOutputted()
        {
        	return true;
        }

        int rank()
        {
        	return rand() % (m_ranksLimit*4);
        }

    private:
        MPI_Comm m_mpiComm;
        int m_ranksLimit;
};


TEST(lumberjack_Logger, combineMessages01)
{
	int ranksLimit = 5;
	TestCommunicator communicator;
	communicator.initialize(ATK_NULLPTR, ranksLimit);
	asctoolkit::lumberjack::Logger logger;
	logger.initialize(&communicator, ranksLimit);

	logger.queueMessage("Should be combined.");
	logger.queueMessage("Should be combined.");
	logger.queueMessage("Should be combined.");
	logger.queueMessage("Should be combined.");
	logger.queueMessage("Should be combined.");
	logger.queueMessage("Should be combined.");

	logger.pushMessagesFully();

	std::vector<asctoolkit::lumberjack::Message*> messages;
	logger.getMessages(messages);

	EXPECT_EQ((int)messages.size(), 1);
	EXPECT_EQ(messages[0]->text(), "Should be combined.");
	EXPECT_EQ(messages[0]->rankCount(), 6);

	logger.finalize();
	communicator.finalize();
}

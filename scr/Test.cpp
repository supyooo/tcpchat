#include<gtest/gtest.h>
#include"serverclient.h"


TEST(MyclassTest, testGetSetName)
{
    AcceptedSocket newClient = AcceptedSocket();

    string name = "Gosha";

    newClient.SetName(name);

    EXPECT_STREQ(name.c_str(), newClient.GetName().c_str());
    
}

TEST(CLIENT_END, test_client_end_function_true_v1)
{
    const char* end = "#";

    bool result = client_end(end);
    
    ASSERT_TRUE(result);
}

TEST(CLIENT_END, test_client_end_function_true_v2)
{
    const char* end = "GJHGkj#";

    bool result = client_end(end);
    
    ASSERT_TRUE(result);
}

TEST(CLIENT_END, test_client_end_function_true_v3)
{
    const char* end = "###";

    bool result = client_end(end);
    
    ASSERT_TRUE(result);
}

TEST(CLIENT_END, test_client_end_function_false)
{
    const char* end = "GJHGkj";

    bool result = client_end(end);
    
    ASSERT_FALSE(result);
}

TEST(MyclassTest, testGetSocket)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;
    socklen_t c_addlen = sizeof(struct sockaddr_in);
    int clientSocket = accept(server, reinterpret_cast<struct sockaddr*>(&client_address), &c_addlen);

    AcceptedSocket acceptedSocket = AcceptedSocket(clientSocket, client_address);


    EXPECT_EQ(clientSocket, acceptedSocket.GetSock());
    
}


int main(int argc, char**argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
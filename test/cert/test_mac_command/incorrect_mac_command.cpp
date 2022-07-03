#include "common.h"

void test_incorrect_mac_command()
{
    // Step 1
    // DUT sends Unconfirmed frame
    auto nextPacket = dut::wait_for_data(defaultWaitTime);
    TEST_ASSERT(check_is_next_packet(nextPacket, server_state));
    TEST_ASSERT(is_data(nextPacket));
    TEST_ASSERT_FALSE(is_confirmed_uplink(nextPacket));

    // The TCL sends Unconfirmed frame
    // MAC-CMD LinkADRReq
    // Payload = [0x]0380000000
    // FPort = 0
    auto nextResponse = make_data_response(
        0, std::vector<uint8_t>{0x03, 0x80, 0x00, 0x00, 0x00}, false, server_state);
    nextResponse.time = nextPacket.time + RECEIVE_DELAY2;
    dut::send_data(nextResponse);

    // Step 2
    // DUT sends Unconfirmed frame
    // LinkADRAns NOT = OK
    nextPacket = dut::wait_for_data(defaultWaitTime);
    TEST_ASSERT(check_is_next_packet(nextPacket, server_state));
    TEST_ASSERT(is_data(nextPacket));
    TEST_ASSERT_FALSE(is_confirmed_uplink(nextPacket));
    auto macResponse = get_mac_command_values(nextPacket, server_state);
    TEST_ASSERT_EQUAL_UINT(2, macResponse.size());
    TEST_ASSERT_EQUAL_UINT8(0x03, macResponse[0]);
    TEST_ASSERT_NOT_EQUAL_UINT8(0b00000111, macResponse[1]);

    // The TCL sends Unconfirmed frame
    // MAC-CMD LinkADRReq
    // Payload = [0x]03010000
    // FPort = 0
    nextResponse = make_data_response(
        0, std::vector<uint8_t>{0x03, 0x01, 0x00, 0x00}, false, server_state);
    nextResponse.time = nextPacket.time + RECEIVE_DELAY2;
    dut::send_data(nextResponse);

    // Step 3
    // DUT sends Unconfirmed frame
    // No response
    nextPacket = dut::wait_for_data(defaultWaitTime);
    TEST_ASSERT(check_is_next_packet(nextPacket, server_state));
    TEST_ASSERT(is_data(nextPacket));
    TEST_ASSERT_FALSE(is_confirmed_uplink(nextPacket));
    macResponse = get_mac_command_values(nextPacket, server_state);
    TEST_ASSERT_EQUAL_UINT(0, macResponse.size());

}
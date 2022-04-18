
packet MyPacket {
   	int game_state;
	int srcAddr = -1;
	int hopcount = 0;
	simtime_t startTime = 0;
	int lastHopAddr = -1;
	
}

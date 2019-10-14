#ifndef CO2_SENSOR_H_
#define CO2_SENSOR_H_


class CO2Sensor {
public:
	CO2Sensor();
	//static CO2Sensor* get_instance();
	unsigned int get_concentration();
	//bool is_connected();
	bool request_concentration();
	void set_response_callback(void (*callback)(unsigned int));

	void receive_next_byte(unsigned char data);
	void send_next_byte();
private:
	//static CO2Sensor* instance;
	unsigned int concentration;
	unsigned int concentration_buffer;
	bool has_request;
	//bool connected;
	void (*response_callback)(unsigned int co2_concentration);

	unsigned char checksum;
	unsigned char response_bytes_counter;
	unsigned char co2_concentration_read_command_offset;

	void response();
	void stop();
};

#endif /* CO2_SENSOR_H_ */

Feature: Application will output data from serial port
	To debug my Mono app
	As a maker
	I want the application to show me the output that Mono sends on the serial port

	Scenario Outline: Should show stream of data from responsive device
		When I run `monoprog --mock <board> <arg>`
		Then the output should contain "<output>"

		Examples:
			| arg      | board        | output                 |
			| -s       | responsive   | Test data from Mono... |
			| --serial | responsive   | Test data from Mono... |

## Airport | Fourth activity
This program simulates an airport with 3 runways and 5 airplanes using FreeRTOS. Each airplane requests to land by sending a message to a queue. 
The control tower receives those requests and assigns a free runway using semaphores to coordinate access.
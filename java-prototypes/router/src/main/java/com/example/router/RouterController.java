package com.example.router;

import org.springframework.web.bind.annotation.*;

@RestController
public class RouterController {

	@GetMapping("/route")
	public String routeRequest(@RequestParam String service) {
		return "Routing to " + service + " (Java prototype of my Go router)";
	}
	
	@GetMapping("/health")
	public String healthCheck() {
		return "Router service UP";
	}
}
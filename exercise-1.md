# Exercise 1
* Context: it's possible that A & B shall be deployed into two separated ECUs/Systems

## Static View
* Entities
  * A
  * B
* Communication
  * Protocol: A (requester) -UDP-> B (consumer)

* Data structures
  * Request
    * 1 byte: Config ID
    * 1 byte: Param ID
    * 1 byte: value

  * Number of configuration sets: 2

## Fault Injection
* B did not convert byte-order of the request package received via UDP => incorrect configuration would be selected

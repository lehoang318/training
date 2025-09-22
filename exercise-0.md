# Exercise 0
## Static View
* Entities
  * A
  * B
* Communication
  * Protocol: A (requester) -shared mem (local)-> B (consumer)

* Data structures
  * Request
  ```
  enum COMMAND {
    GET = 0,
    SET
  };
  struct set_request {
    uint8 cmd;            // COMMAND: `SET`
    uint8 cfg_id;         // Config ID
    uint8 param_id;       // Param ID
    uint8 val;            // New value
    uint64 timestamp_us;  // Request timestamp in us (utc)
  }

  struct get_request {
    uint8 cmd;      // COMMAND: `GET`
    uint8 cfg_id;   // Config ID
    uint8 param_id; // Param ID
  }

  struct get_response {
    uint8 cmd;      // COMMAND: `GET`
    uint8 cfg_id;   // Config ID
    uint8 param_id; // Param ID
    uint8 val;      // New value
    uint64 timestamp_us;  // last changed timestamp_us
  }

  ```

  * Number of configuration sets: 2
  ```
  uint8 config[2][16];  // 2 configuration set, 16 params/set
  uint64 timestamp_us;  // last changed timestamp in us (utc)
  ```

## Defects
* Sending
  ```
  struct request req;
  req.cfg_id = 1;
  req.param_id = 5;
  req.val = 10;
  com_send(&req);
  ```

* Receiving
  ```
  struct request req;
  com_recv(&req);

  config[req.cfg_id][req.param_id] = req.val;
  ```

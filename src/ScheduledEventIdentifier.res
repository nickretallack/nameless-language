type t =
  | IntervalID(Js.Global.intervalId)
  | TimeoutID(Js.Global.timeoutId)
  | AnimationFrameRequestID(AnimationFrame.requestID)

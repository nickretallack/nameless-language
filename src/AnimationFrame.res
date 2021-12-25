type requestID = int

@val external request: (unit => unit) => requestID = "requestAnimationFrame"
@val external cancel: requestID => unit = "cancelAnimationFrame"

let f = (hashedContent: HashedContent.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("contentID", string(hashedContent.contentID)),
    ("canonicalString", string(hashedContent.canonicalString))})
}
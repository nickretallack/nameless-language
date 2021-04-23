let f = (list, fn) => React.array(Array.of_list(List.mapi(fn, list)))

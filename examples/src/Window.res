type listenerId

@val @scope("window")
external addWindowEventListener: (string, 'a => unit) => listenerId = "addEventListener"

@val @scope("window")
external removeWindowEventListener: (string, listenerId) => unit = "removeEventListener"

@val @scope("window") external windowWidth: int = "innerWidth"
@val @scope("window") external windowHeight: int = "innerHeight"

let useWindowSize = () => {
  let ((width, height), setSize) = React.useState(() => (windowWidth, windowHeight))
  React.useEffect0(() => {
    let handleResize = _ => setSize(_ => (windowWidth, windowHeight))
    let listener = addWindowEventListener("resize", handleResize)
    Some(() => removeWindowEventListener("resize", listener))
  })
  (width, height)
}

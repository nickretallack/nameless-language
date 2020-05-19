# Nameless Language

This is a development environment and interpreter for a visual programming language I came up with. [You can read more about it here.](https://medium.com/@nickretallack/creating-nameless-an-accessible-visual-programming-language-1a8984c5478a)

## Develop

```sh
npm install
npm run server
# in a new tab
npm start
```

Visit `http://localhost:8000/` in your browser. Change any `.re` file in `src` to see the page auto-reload.

## Bundle for Production

Just run webpack. It will generate files used by `docs/index.html` which is served by GiHhub Pages.

```sh
./node_modules/.bin/webpack
```

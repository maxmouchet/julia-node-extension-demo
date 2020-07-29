# Example Express API

## Installation

```bash
# To install using the pre-built extension on npm/S3
npm install

# Alternatively, to install using the extension in this repository
# 1) In julia-node-extension-demo/
npm link

# 2) In julia-node-extension-demo/example/
npm link @maxmouchet/julia-node-extension-demo
npm install
```

## Usage

```bash
node app.js
# Example app listening at http://localhost:3000
```

```bash
node test.js
# POST http://localhost:3000/kmeans
# {
#   data: [
#     [ 1.3303511126686933, 2.160981496533918 ],
#     [ -0.9983808670292756, 0.555019906506169 ],
#     [ 1.6727414680901176, 2.377881858630246 ],
#     [ 1.7091669876224176, 2.4520727549913963 ],
#     [ 1.4430257088286245, 2.075431167052574 ],
#     [ 0.8094185008297733, 1.9919532343063044 ],
#     [ 1.1659808653837864, 1.6961459199717197 ],
#     [ 1.9585840751297692, 2.3579541341624912 ],
#     [ 1.1792036234160181, 0.08744255340918461 ],
#     [ -0.38137260771751463, -0.10968433449772902 ]
#   ],
#   k: 3
# }
# {
#   assignments: [
#     1, 2, 3, 3, 1,
#     1, 1, 3, 2, 2
#   ],
#   centers: [
#     [ 1.1871940469277193, 1.981127954466129 ],
#     [ -0.06684995044359071, 0.1775927084725415 ],
#     [ 1.7801641769474348, 2.395969582594711 ]
#   ],
#   converged: true,
#   iterations: 2
# }

```

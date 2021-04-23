set -e
for filename in `ls src/*.re`
do
node_modules/.bin/bsc -format $filename > ${filename}2
mv ${filename}2 $filename
git mv $filename ${filename%.re}.res
done
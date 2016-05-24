#!/bin/bash
cd rte-www
echo "Copying logs"
cp ../rte/logs/* ./logs/.

git pull
if [[ ${OV_ARCH_BITWIDTH} == "32" && ${CC} == "gcc" ]]; then 
  echo "Cleaning up log dir (leave the 50 newest)"
  mkdir ./logs/tmpSafe
  mv -v `find ./logs/ -maxdepth 1  -name 'acplt_build*.log' -type f -printf '%T@ %p\n' | sort -n | tail -50 | cut -f2- -d" " ` ./logs/tmpSafe/.
  rm -v `find ./logs/ -maxdepth 1 -name 'acplt_build*.log' -type f`
  mv -v ./logs/tmpSafe/* ./logs/.
  rm -vR ./logs/tmpSafe
  cd ./releases
  bash ../../rte/tools/travis_createListPage.sh
  cd ..
fi

git add -A ./logs/*
echo "Pushing to github"
git config --global user.email "rte_bot@outlook.de"
git config --global user.name "rte-bot"
git config --global push.default simple
git commit -am "updated generated documentation (texinfo) on webpage and created releases by travis-ci [ci skip] (Job: ${TRAVIS_JOB_NUMBER})"
git push -q https://$GH_TOKEN:x-oauth-basic@github.com/acplt/rte-www
cd ..

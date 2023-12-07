#!/bin/bash

###############################################################################

readonly GITLAB_SERVER_GIT="git@109.236.85.111"
readonly GITLAB_SERVER_HTTP="http://109.236.85.111/gitlab"
readonly GITLAB_SERVER_HTTPS="https://109.236.85.111/gitlab"
readonly GITHUB_SERVER_HTTPS="https://github.com"

function clone_or_pull {
    local readonly user_name="$1"
    local readonly repo_name="$2"
    local readonly repo_path="$3"
    local readonly repo_flag=$4
    
    local repo_url=
    if [[ $repo_name == workgit:* ]]; then
        repo_name=${repo_name#workgit:}
        repo_url="$GITLAB_SERVER_GIT:$user_name/$repo_name.git"
    elif [[ $repo_name == workhttp:* ]]; then
        repo_name=${repo_name#workhttp:}
        repo_url="$GITLAB_SERVER_HTTP/$user_name/$repo_name.git"
    elif [[ $repo_name == workhttps:* ]]; then
        repo_name=${repo_name#workhttps:}
        repo_url="$GITLAB_SERVER_HTTPS/$user_name/$repo_name.git"
    elif [[ $repo_name == github:* ]]; then
        repo_name=${repo_name#github:}
        repo_url="$GITHUB_SERVER_HTTPS/$user_name/$repo_name.git"
    else
        echo "bad repo_name: $repo_name"
        exit 1
    fi
    #echo "repo_name=$repo_name"
    #echo "repo_url =$repo_url"
    
    echo "#######################################################"
    [[ ! -e "$repo_path" ]] && {
        mkdir -p "$repo_path"
    }

    local res=0
    echo "checking $repo_name ... "
    [[ -d "$repo_path/$repo_name" ]] && {
        (cd "$repo_path/$repo_name" && git pull)
        res=$?
        [[ $res == 0 ]] && {
            echo -n
        } || {
            echo "pull error($res)!"
            exit $res
        }
    } || {
        [[ $repo_flag == 1 ]] && {
            local ppath="$PWD"
            [[ ! -d "$repo_path" ]] && {
                mkdir -p "$repo_path"
            }
            cd "$repo_path"
        }

        git clone $repo_url
        res=$?
        [[ $res != 0 ]] && {
            echo "clone error($res)"
            exit $res
        } || {
            echo -n
        }

        [[ $repo_flag == 1 ]] && {
            cd "$ppath"
        }
    }
}

###############################################################################

clone_or_pull \
    "tasheehoo" \
    "workgit:yas" \
    "../include" \
    1

<p align="center"><img src="img/juniper_logo.png" width=250, height=250></p>

# Juniper Version Control

Juniper is a Version Control System written in C++ using zlib for file compression, and OpenSSL for hash generation. It is based off of one of my favorite dev tools, git.

Note that the hash generation is purely used as an identifier, and has no functional purpose in storing the object file data.

## Overview

To add files to the staging area, use `juniper add filepath`, or `juniper add dirpath`.

Once you have added the files to the staging area, commit it to the database using `juniper commit "commit message"`. Just like that, you've saved your work to the database with a helpful commit message.

If you wish to rollback to a previous commit, take the commit hash and run `juniper checkout commit-hash`.


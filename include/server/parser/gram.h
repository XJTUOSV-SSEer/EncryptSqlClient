/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     UIDENT = 259,
     FCONST = 260,
     SCONST = 261,
     USCONST = 262,
     BCONST = 263,
     XCONST = 264,
     Op = 265,
     ICONST = 266,
     PARAM = 267,
     TYPECAST = 268,
     DOT_DOT = 269,
     COLON_EQUALS = 270,
     EQUALS_GREATER = 271,
     LESS_EQUALS = 272,
     GREATER_EQUALS = 273,
     NOT_EQUALS = 274,
     ABORT_P = 275,
     ABSOLUTE_P = 276,
     ACCESS = 277,
     ACTION = 278,
     ADD_P = 279,
     ADMIN = 280,
     AFTER = 281,
     AGGREGATE = 282,
     ALL = 283,
     ALSO = 284,
     ALTER = 285,
     ALWAYS = 286,
     ANALYSE = 287,
     ANALYZE = 288,
     AND = 289,
     ANY = 290,
     ARRAY = 291,
     AS = 292,
     ASC = 293,
     ASENSITIVE = 294,
     ASSERTION = 295,
     ASSIGNMENT = 296,
     ASYMMETRIC = 297,
     ATOMIC = 298,
     AT = 299,
     ATTACH = 300,
     ATTRIBUTE = 301,
     AUTHORIZATION = 302,
     BACKWARD = 303,
     BEFORE = 304,
     BEGIN_P = 305,
     BETWEEN = 306,
     BIGINT = 307,
     BINARY = 308,
     BIT = 309,
     BOOLEAN_P = 310,
     BOTH = 311,
     BREADTH = 312,
     BY = 313,
     CACHE = 314,
     CALL = 315,
     CALLED = 316,
     CASCADE = 317,
     CASCADED = 318,
     CASE = 319,
     CAST = 320,
     CATALOG_P = 321,
     CHAIN = 322,
     CHAR_P = 323,
     CHARACTER = 324,
     CHARACTERISTICS = 325,
     CHECK = 326,
     CHECKPOINT = 327,
     CLASS = 328,
     CLOSE = 329,
     CLUSTER = 330,
     COALESCE = 331,
     COLLATE = 332,
     COLLATION = 333,
     COLUMN = 334,
     COLUMNS = 335,
     COMMENT = 336,
     COMMENTS = 337,
     COMMIT = 338,
     COMMITTED = 339,
     COMPRESSION = 340,
     CONCURRENTLY = 341,
     CONFIGURATION = 342,
     CONFLICT = 343,
     CONNECTION = 344,
     CONSTRAINT = 345,
     CONSTRAINTS = 346,
     CONTENT_P = 347,
     CONTINUE_P = 348,
     CONVERSION_P = 349,
     COPY = 350,
     COST = 351,
     CREATE = 352,
     CROSS = 353,
     CSV = 354,
     CUBE = 355,
     CURRENT_P = 356,
     CURRENT_CATALOG = 357,
     CURRENT_DATE = 358,
     CURRENT_ROLE = 359,
     CURRENT_SCHEMA = 360,
     CURRENT_TIME = 361,
     CURRENT_TIMESTAMP = 362,
     CURRENT_USER = 363,
     CURSOR = 364,
     CYCLE = 365,
     DATA_P = 366,
     DATABASE = 367,
     DAY_P = 368,
     DEALLOCATE = 369,
     DEC = 370,
     DECIMAL_P = 371,
     DECLARE = 372,
     DEFAULT = 373,
     DEFAULTS = 374,
     DEFERRABLE = 375,
     DEFERRED = 376,
     DEFINER = 377,
     DELETE_P = 378,
     DELIMITER = 379,
     DELIMITERS = 380,
     DEPENDS = 381,
     DEPTH = 382,
     DESC = 383,
     DETACH = 384,
     DICTIONARY = 385,
     DISABLE_P = 386,
     DISCARD = 387,
     DISTINCT = 388,
     DO = 389,
     DOCUMENT_P = 390,
     DOMAIN_P = 391,
     DOUBLE_P = 392,
     DROP = 393,
     EACH = 394,
     ELSE = 395,
     ENABLE_P = 396,
     ENCODING = 397,
     ENCRYPTED = 398,
     END_P = 399,
     ENUM_P = 400,
     ESCAPE = 401,
     EVENT = 402,
     EXCEPT = 403,
     EXCLUDE = 404,
     EXCLUDING = 405,
     EXCLUSIVE = 406,
     EXECUTE = 407,
     EXISTS = 408,
     EXPLAIN = 409,
     EXPRESSION = 410,
     EXTENSION = 411,
     EXTERNAL = 412,
     EXTRACT = 413,
     FALSE_P = 414,
     FAMILY = 415,
     FETCH = 416,
     FILTER = 417,
     FINALIZE = 418,
     FIRST_P = 419,
     FLOAT_P = 420,
     FOLLOWING = 421,
     FOR = 422,
     FORCE = 423,
     FOREIGN = 424,
     FORWARD = 425,
     FREEZE = 426,
     FROM = 427,
     FULL = 428,
     FUNCTION = 429,
     FUNCTIONS = 430,
     GENERATED = 431,
     GLOBAL = 432,
     GRANT = 433,
     GRANTED = 434,
     GREATEST = 435,
     GROUP_P = 436,
     GROUPING = 437,
     GROUPS = 438,
     HANDLER = 439,
     HAVING = 440,
     HEADER_P = 441,
     HOLD = 442,
     HOUR_P = 443,
     IDENTITY_P = 444,
     IF_P = 445,
     ILIKE = 446,
     IMMEDIATE = 447,
     IMMUTABLE = 448,
     IMPLICIT_P = 449,
     IMPORT_P = 450,
     IN_P = 451,
     INCLUDE = 452,
     INCLUDING = 453,
     INCREMENT = 454,
     INDEX = 455,
     INDEXES = 456,
     INHERIT = 457,
     INHERITS = 458,
     INITIALLY = 459,
     INLINE_P = 460,
     INNER_P = 461,
     INOUT = 462,
     INPUT_P = 463,
     INSENSITIVE = 464,
     INSERT = 465,
     INSTEAD = 466,
     INT_P = 467,
     INTEGER = 468,
     INTERSECT = 469,
     INTERVAL = 470,
     INTO = 471,
     INVOKER = 472,
     IS = 473,
     ISNULL = 474,
     ISOLATION = 475,
     JOIN = 476,
     KEY = 477,
     LABEL = 478,
     LANGUAGE = 479,
     LARGE_P = 480,
     LAST_P = 481,
     LATERAL_P = 482,
     LEADING = 483,
     LEAKPROOF = 484,
     LEAST = 485,
     LEFT = 486,
     LEVEL = 487,
     LIKE = 488,
     LIMIT = 489,
     LISTEN = 490,
     LOAD = 491,
     LOCAL = 492,
     LOCALTIME = 493,
     LOCALTIMESTAMP = 494,
     LOCATION = 495,
     LOCK_P = 496,
     LOCKED = 497,
     LOGGED = 498,
     MAPPING = 499,
     MATCH = 500,
     MATERIALIZED = 501,
     MAXVALUE = 502,
     METHOD = 503,
     MINUTE_P = 504,
     MINVALUE = 505,
     MODE = 506,
     MONTH_P = 507,
     MOVE = 508,
     NAME_P = 509,
     NAMES = 510,
     NATIONAL = 511,
     NATURAL = 512,
     NCHAR = 513,
     NEW = 514,
     NEXT = 515,
     NFC = 516,
     NFD = 517,
     NFKC = 518,
     NFKD = 519,
     NO = 520,
     NONE = 521,
     NORMALIZE = 522,
     NORMALIZED = 523,
     NOT = 524,
     NOTHING = 525,
     NOTIFY = 526,
     NOTNULL = 527,
     NOWAIT = 528,
     NULL_P = 529,
     NULLIF = 530,
     NULLS_P = 531,
     NUMERIC = 532,
     OBJECT_P = 533,
     OF = 534,
     OFF = 535,
     OFFSET = 536,
     OIDS = 537,
     OLD = 538,
     ON = 539,
     ONLY = 540,
     OPERATOR = 541,
     OPTION = 542,
     OPTIONS = 543,
     OR = 544,
     ORDER = 545,
     ORDINALITY = 546,
     OTHERS = 547,
     OUT_P = 548,
     OUTER_P = 549,
     OVER = 550,
     OVERLAPS = 551,
     OVERLAY = 552,
     OVERRIDING = 553,
     OWNED = 554,
     OWNER = 555,
     PARALLEL = 556,
     PARSER = 557,
     PARTIAL = 558,
     PARTITION = 559,
     PASSING = 560,
     PASSWORD = 561,
     PLACING = 562,
     PLANS = 563,
     POLICY = 564,
     POSITION = 565,
     PRECEDING = 566,
     PRECISION = 567,
     PRESERVE = 568,
     PREPARE = 569,
     PREPARED = 570,
     PRIMARY = 571,
     PRIOR = 572,
     PRIVILEGES = 573,
     PROCEDURAL = 574,
     PROCEDURE = 575,
     PROCEDURES = 576,
     PROGRAM = 577,
     PUBLICATION = 578,
     QUOTE = 579,
     RANGE = 580,
     READ = 581,
     REAL = 582,
     REASSIGN = 583,
     RECHECK = 584,
     RECURSIVE = 585,
     REF_P = 586,
     REFERENCES = 587,
     REFERENCING = 588,
     REFRESH = 589,
     REINDEX = 590,
     RELATIVE_P = 591,
     RELEASE = 592,
     RENAME = 593,
     REPEATABLE = 594,
     REPLACE = 595,
     REPLICA = 596,
     RESET = 597,
     RESTART = 598,
     RESTRICT = 599,
     RETURN = 600,
     RETURNING = 601,
     RETURNS = 602,
     REVOKE = 603,
     RIGHT = 604,
     ROLE = 605,
     ROLLBACK = 606,
     ROLLUP = 607,
     ROUTINE = 608,
     ROUTINES = 609,
     ROW = 610,
     ROWS = 611,
     RULE = 612,
     SAVEPOINT = 613,
     SCHEMA = 614,
     SCHEMAS = 615,
     SCROLL = 616,
     SEARCH = 617,
     SECOND_P = 618,
     SECURITY = 619,
     SELECT = 620,
     SEQUENCE = 621,
     SEQUENCES = 622,
     SERIALIZABLE = 623,
     SERVER = 624,
     SESSION = 625,
     SESSION_USER = 626,
     SET = 627,
     SETS = 628,
     SETOF = 629,
     SHARE = 630,
     SHOW = 631,
     SIMILAR = 632,
     SIMPLE = 633,
     SKIP = 634,
     SMALLINT = 635,
     SNAPSHOT = 636,
     SOME = 637,
     SQL_P = 638,
     STABLE = 639,
     STANDALONE_P = 640,
     START = 641,
     STATEMENT = 642,
     STATISTICS = 643,
     STDIN = 644,
     STDOUT = 645,
     STORAGE = 646,
     STORED = 647,
     STRICT_P = 648,
     STRIP_P = 649,
     SUBSCRIPTION = 650,
     SUBSTRING = 651,
     SUPPORT = 652,
     SYMMETRIC = 653,
     SYSID = 654,
     SYSTEM_P = 655,
     TABLE = 656,
     TABLES = 657,
     TABLESAMPLE = 658,
     TABLESPACE = 659,
     TEMP = 660,
     TEMPLATE = 661,
     TEMPORARY = 662,
     TEXT_P = 663,
     THEN = 664,
     TIES = 665,
     TIME = 666,
     TIMESTAMP = 667,
     TO = 668,
     TRAILING = 669,
     TRANSACTION = 670,
     TRANSFORM = 671,
     TREAT = 672,
     TRIGGER = 673,
     TRIM = 674,
     TRUE_P = 675,
     TRUNCATE = 676,
     TRUSTED = 677,
     TYPE_P = 678,
     TYPES_P = 679,
     UESCAPE = 680,
     UNBOUNDED = 681,
     UNCOMMITTED = 682,
     UNENCRYPTED = 683,
     UNION = 684,
     UNIQUE = 685,
     UNKNOWN = 686,
     UNLISTEN = 687,
     UNLOGGED = 688,
     UNTIL = 689,
     UPDATE = 690,
     USER = 691,
     USING = 692,
     VACUUM = 693,
     VALID = 694,
     VALIDATE = 695,
     VALIDATOR = 696,
     VALUE_P = 697,
     VALUES = 698,
     VARCHAR = 699,
     VARIADIC = 700,
     VARYING = 701,
     VERBOSE = 702,
     VERSION_P = 703,
     VIEW = 704,
     VIEWS = 705,
     VOLATILE = 706,
     WHEN = 707,
     WHERE = 708,
     WHITESPACE_P = 709,
     WINDOW = 710,
     WITH = 711,
     WITHIN = 712,
     WITHOUT = 713,
     WORK = 714,
     WRAPPER = 715,
     WRITE = 716,
     XML_P = 717,
     XMLATTRIBUTES = 718,
     XMLCONCAT = 719,
     XMLELEMENT = 720,
     XMLEXISTS = 721,
     XMLFOREST = 722,
     XMLNAMESPACES = 723,
     XMLPARSE = 724,
     XMLPI = 725,
     XMLROOT = 726,
     XMLSERIALIZE = 727,
     XMLTABLE = 728,
     YEAR_P = 729,
     YES_P = 730,
     ZONE = 731,
     NOT_LA = 732,
     NULLS_LA = 733,
     WITH_LA = 734,
     MODE_TYPE_NAME = 735,
     MODE_PLPGSQL_EXPR = 736,
     MODE_PLPGSQL_ASSIGN1 = 737,
     MODE_PLPGSQL_ASSIGN2 = 738,
     MODE_PLPGSQL_ASSIGN3 = 739,
     UMINUS = 740
   };
#endif
/* Tokens.  */
#define IDENT 258
#define UIDENT 259
#define FCONST 260
#define SCONST 261
#define USCONST 262
#define BCONST 263
#define XCONST 264
#define Op 265
#define ICONST 266
#define PARAM 267
#define TYPECAST 268
#define DOT_DOT 269
#define COLON_EQUALS 270
#define EQUALS_GREATER 271
#define LESS_EQUALS 272
#define GREATER_EQUALS 273
#define NOT_EQUALS 274
#define ABORT_P 275
#define ABSOLUTE_P 276
#define ACCESS 277
#define ACTION 278
#define ADD_P 279
#define ADMIN 280
#define AFTER 281
#define AGGREGATE 282
#define ALL 283
#define ALSO 284
#define ALTER 285
#define ALWAYS 286
#define ANALYSE 287
#define ANALYZE 288
#define AND 289
#define ANY 290
#define ARRAY 291
#define AS 292
#define ASC 293
#define ASENSITIVE 294
#define ASSERTION 295
#define ASSIGNMENT 296
#define ASYMMETRIC 297
#define ATOMIC 298
#define AT 299
#define ATTACH 300
#define ATTRIBUTE 301
#define AUTHORIZATION 302
#define BACKWARD 303
#define BEFORE 304
#define BEGIN_P 305
#define BETWEEN 306
#define BIGINT 307
#define BINARY 308
#define BIT 309
#define BOOLEAN_P 310
#define BOTH 311
#define BREADTH 312
#define BY 313
#define CACHE 314
#define CALL 315
#define CALLED 316
#define CASCADE 317
#define CASCADED 318
#define CASE 319
#define CAST 320
#define CATALOG_P 321
#define CHAIN 322
#define CHAR_P 323
#define CHARACTER 324
#define CHARACTERISTICS 325
#define CHECK 326
#define CHECKPOINT 327
#define CLASS 328
#define CLOSE 329
#define CLUSTER 330
#define COALESCE 331
#define COLLATE 332
#define COLLATION 333
#define COLUMN 334
#define COLUMNS 335
#define COMMENT 336
#define COMMENTS 337
#define COMMIT 338
#define COMMITTED 339
#define COMPRESSION 340
#define CONCURRENTLY 341
#define CONFIGURATION 342
#define CONFLICT 343
#define CONNECTION 344
#define CONSTRAINT 345
#define CONSTRAINTS 346
#define CONTENT_P 347
#define CONTINUE_P 348
#define CONVERSION_P 349
#define COPY 350
#define COST 351
#define CREATE 352
#define CROSS 353
#define CSV 354
#define CUBE 355
#define CURRENT_P 356
#define CURRENT_CATALOG 357
#define CURRENT_DATE 358
#define CURRENT_ROLE 359
#define CURRENT_SCHEMA 360
#define CURRENT_TIME 361
#define CURRENT_TIMESTAMP 362
#define CURRENT_USER 363
#define CURSOR 364
#define CYCLE 365
#define DATA_P 366
#define DATABASE 367
#define DAY_P 368
#define DEALLOCATE 369
#define DEC 370
#define DECIMAL_P 371
#define DECLARE 372
#define DEFAULT 373
#define DEFAULTS 374
#define DEFERRABLE 375
#define DEFERRED 376
#define DEFINER 377
#define DELETE_P 378
#define DELIMITER 379
#define DELIMITERS 380
#define DEPENDS 381
#define DEPTH 382
#define DESC 383
#define DETACH 384
#define DICTIONARY 385
#define DISABLE_P 386
#define DISCARD 387
#define DISTINCT 388
#define DO 389
#define DOCUMENT_P 390
#define DOMAIN_P 391
#define DOUBLE_P 392
#define DROP 393
#define EACH 394
#define ELSE 395
#define ENABLE_P 396
#define ENCODING 397
#define ENCRYPTED 398
#define END_P 399
#define ENUM_P 400
#define ESCAPE 401
#define EVENT 402
#define EXCEPT 403
#define EXCLUDE 404
#define EXCLUDING 405
#define EXCLUSIVE 406
#define EXECUTE 407
#define EXISTS 408
#define EXPLAIN 409
#define EXPRESSION 410
#define EXTENSION 411
#define EXTERNAL 412
#define EXTRACT 413
#define FALSE_P 414
#define FAMILY 415
#define FETCH 416
#define FILTER 417
#define FINALIZE 418
#define FIRST_P 419
#define FLOAT_P 420
#define FOLLOWING 421
#define FOR 422
#define FORCE 423
#define FOREIGN 424
#define FORWARD 425
#define FREEZE 426
#define FROM 427
#define FULL 428
#define FUNCTION 429
#define FUNCTIONS 430
#define GENERATED 431
#define GLOBAL 432
#define GRANT 433
#define GRANTED 434
#define GREATEST 435
#define GROUP_P 436
#define GROUPING 437
#define GROUPS 438
#define HANDLER 439
#define HAVING 440
#define HEADER_P 441
#define HOLD 442
#define HOUR_P 443
#define IDENTITY_P 444
#define IF_P 445
#define ILIKE 446
#define IMMEDIATE 447
#define IMMUTABLE 448
#define IMPLICIT_P 449
#define IMPORT_P 450
#define IN_P 451
#define INCLUDE 452
#define INCLUDING 453
#define INCREMENT 454
#define INDEX 455
#define INDEXES 456
#define INHERIT 457
#define INHERITS 458
#define INITIALLY 459
#define INLINE_P 460
#define INNER_P 461
#define INOUT 462
#define INPUT_P 463
#define INSENSITIVE 464
#define INSERT 465
#define INSTEAD 466
#define INT_P 467
#define INTEGER 468
#define INTERSECT 469
#define INTERVAL 470
#define INTO 471
#define INVOKER 472
#define IS 473
#define ISNULL 474
#define ISOLATION 475
#define JOIN 476
#define KEY 477
#define LABEL 478
#define LANGUAGE 479
#define LARGE_P 480
#define LAST_P 481
#define LATERAL_P 482
#define LEADING 483
#define LEAKPROOF 484
#define LEAST 485
#define LEFT 486
#define LEVEL 487
#define LIKE 488
#define LIMIT 489
#define LISTEN 490
#define LOAD 491
#define LOCAL 492
#define LOCALTIME 493
#define LOCALTIMESTAMP 494
#define LOCATION 495
#define LOCK_P 496
#define LOCKED 497
#define LOGGED 498
#define MAPPING 499
#define MATCH 500
#define MATERIALIZED 501
#define MAXVALUE 502
#define METHOD 503
#define MINUTE_P 504
#define MINVALUE 505
#define MODE 506
#define MONTH_P 507
#define MOVE 508
#define NAME_P 509
#define NAMES 510
#define NATIONAL 511
#define NATURAL 512
#define NCHAR 513
#define NEW 514
#define NEXT 515
#define NFC 516
#define NFD 517
#define NFKC 518
#define NFKD 519
#define NO 520
#define NONE 521
#define NORMALIZE 522
#define NORMALIZED 523
#define NOT 524
#define NOTHING 525
#define NOTIFY 526
#define NOTNULL 527
#define NOWAIT 528
#define NULL_P 529
#define NULLIF 530
#define NULLS_P 531
#define NUMERIC 532
#define OBJECT_P 533
#define OF 534
#define OFF 535
#define OFFSET 536
#define OIDS 537
#define OLD 538
#define ON 539
#define ONLY 540
#define OPERATOR 541
#define OPTION 542
#define OPTIONS 543
#define OR 544
#define ORDER 545
#define ORDINALITY 546
#define OTHERS 547
#define OUT_P 548
#define OUTER_P 549
#define OVER 550
#define OVERLAPS 551
#define OVERLAY 552
#define OVERRIDING 553
#define OWNED 554
#define OWNER 555
#define PARALLEL 556
#define PARSER 557
#define PARTIAL 558
#define PARTITION 559
#define PASSING 560
#define PASSWORD 561
#define PLACING 562
#define PLANS 563
#define POLICY 564
#define POSITION 565
#define PRECEDING 566
#define PRECISION 567
#define PRESERVE 568
#define PREPARE 569
#define PREPARED 570
#define PRIMARY 571
#define PRIOR 572
#define PRIVILEGES 573
#define PROCEDURAL 574
#define PROCEDURE 575
#define PROCEDURES 576
#define PROGRAM 577
#define PUBLICATION 578
#define QUOTE 579
#define RANGE 580
#define READ 581
#define REAL 582
#define REASSIGN 583
#define RECHECK 584
#define RECURSIVE 585
#define REF_P 586
#define REFERENCES 587
#define REFERENCING 588
#define REFRESH 589
#define REINDEX 590
#define RELATIVE_P 591
#define RELEASE 592
#define RENAME 593
#define REPEATABLE 594
#define REPLACE 595
#define REPLICA 596
#define RESET 597
#define RESTART 598
#define RESTRICT 599
#define RETURN 600
#define RETURNING 601
#define RETURNS 602
#define REVOKE 603
#define RIGHT 604
#define ROLE 605
#define ROLLBACK 606
#define ROLLUP 607
#define ROUTINE 608
#define ROUTINES 609
#define ROW 610
#define ROWS 611
#define RULE 612
#define SAVEPOINT 613
#define SCHEMA 614
#define SCHEMAS 615
#define SCROLL 616
#define SEARCH 617
#define SECOND_P 618
#define SECURITY 619
#define SELECT 620
#define SEQUENCE 621
#define SEQUENCES 622
#define SERIALIZABLE 623
#define SERVER 624
#define SESSION 625
#define SESSION_USER 626
#define SET 627
#define SETS 628
#define SETOF 629
#define SHARE 630
#define SHOW 631
#define SIMILAR 632
#define SIMPLE 633
#define SKIP 634
#define SMALLINT 635
#define SNAPSHOT 636
#define SOME 637
#define SQL_P 638
#define STABLE 639
#define STANDALONE_P 640
#define START 641
#define STATEMENT 642
#define STATISTICS 643
#define STDIN 644
#define STDOUT 645
#define STORAGE 646
#define STORED 647
#define STRICT_P 648
#define STRIP_P 649
#define SUBSCRIPTION 650
#define SUBSTRING 651
#define SUPPORT 652
#define SYMMETRIC 653
#define SYSID 654
#define SYSTEM_P 655
#define TABLE 656
#define TABLES 657
#define TABLESAMPLE 658
#define TABLESPACE 659
#define TEMP 660
#define TEMPLATE 661
#define TEMPORARY 662
#define TEXT_P 663
#define THEN 664
#define TIES 665
#define TIME 666
#define TIMESTAMP 667
#define TO 668
#define TRAILING 669
#define TRANSACTION 670
#define TRANSFORM 671
#define TREAT 672
#define TRIGGER 673
#define TRIM 674
#define TRUE_P 675
#define TRUNCATE 676
#define TRUSTED 677
#define TYPE_P 678
#define TYPES_P 679
#define UESCAPE 680
#define UNBOUNDED 681
#define UNCOMMITTED 682
#define UNENCRYPTED 683
#define UNION 684
#define UNIQUE 685
#define UNKNOWN 686
#define UNLISTEN 687
#define UNLOGGED 688
#define UNTIL 689
#define UPDATE 690
#define USER 691
#define USING 692
#define VACUUM 693
#define VALID 694
#define VALIDATE 695
#define VALIDATOR 696
#define VALUE_P 697
#define VALUES 698
#define VARCHAR 699
#define VARIADIC 700
#define VARYING 701
#define VERBOSE 702
#define VERSION_P 703
#define VIEW 704
#define VIEWS 705
#define VOLATILE 706
#define WHEN 707
#define WHERE 708
#define WHITESPACE_P 709
#define WINDOW 710
#define WITH 711
#define WITHIN 712
#define WITHOUT 713
#define WORK 714
#define WRAPPER 715
#define WRITE 716
#define XML_P 717
#define XMLATTRIBUTES 718
#define XMLCONCAT 719
#define XMLELEMENT 720
#define XMLEXISTS 721
#define XMLFOREST 722
#define XMLNAMESPACES 723
#define XMLPARSE 724
#define XMLPI 725
#define XMLROOT 726
#define XMLSERIALIZE 727
#define XMLTABLE 728
#define YEAR_P 729
#define YES_P 730
#define ZONE 731
#define NOT_LA 732
#define NULLS_LA 733
#define WITH_LA 734
#define MODE_TYPE_NAME 735
#define MODE_PLPGSQL_EXPR 736
#define MODE_PLPGSQL_ASSIGN1 737
#define MODE_PLPGSQL_ASSIGN2 738
#define MODE_PLPGSQL_ASSIGN3 739
#define UMINUS 740




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 217 "gram.y"
{
	core_YYSTYPE		core_yystype;
	/* these fields must match core_YYSTYPE: */
	int					ival;
	char				*str;
	const char			*keyword;

	char				chr;
	bool				boolean;
	JoinType			jtype;
	DropBehavior		dbehavior;
	OnCommitAction		oncommit;
	List				*list;
	Node				*node;
	Value				*value;
	ObjectType			objtype;
	TypeName			*typnam;
	FunctionParameter   *fun_param;
	FunctionParameterMode fun_param_mode;
	ObjectWithArgs		*objwithargs;
	DefElem				*defelt;
	SortBy				*sortby;
	WindowDef			*windef;
	JoinExpr			*jexpr;
	IndexElem			*ielem;
	StatsElem			*selem;
	Alias				*alias;
	RangeVar			*range;
	IntoClause			*into;
	WithClause			*with;
	InferClause			*infer;
	OnConflictClause	*onconflict;
	A_Indices			*aind;
	ResTarget			*target;
	struct PrivTarget	*privtarget;
	AccessPriv			*accesspriv;
	struct ImportQual	*importqual;
	InsertStmt			*istmt;
	VariableSetStmt		*vsetstmt;
	PartitionElem		*partelem;
	PartitionSpec		*partspec;
	PartitionBoundSpec	*partboundspec;
	RoleSpec			*rolespec;
	struct SelectLimit	*selectlimit;
	SetQuantifier	 setquantifier;
	struct GroupClause  *groupclause;
}
/* Line 1529 of yacc.c.  */
#line 1067 "gram.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



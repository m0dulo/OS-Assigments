#ifndef SRC_LYX_UTLIS_ArgumentParser_H_
#define SRC_LYX_UTLIS_ArgumentParser_H_

#pragma once;

#include <vector>
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <string.h>

namespace lyx_utlis {

extern bool verbose, VERBOSE;

class ArgumentParser {
private:
    class Argument_target;

    class FlagTarget;
    class DoubleTarget;
    class IntTarget;
    class UIntTarget;
    class StringTarget;
    class CharTarget;
    class StringVectorTarget;

public:
    ArgumentParser();
    void new_flag(const char *key, const char *long_name, const char *description,bool &dest);

    void new_string( const char *arg_description, const char *description, std::string &dest);
    void new_named_string(const char *key, const char *long_name,
                          const char *arg_description,
                          const char *description,  std::string &dest);
    void new_optional_string( const char *arg_description, const char *description, std::string &dest);

    void new_int( const char *arg_description, const char *description, int &dest);
    void new_named_int(const char *key, const char *long_name,const char *value_name,
                       const char *description,
                       int &dest);
    void new_optional_int(const char *value_name,
                          const char *description,
                          int &dest);
    void new_double(const char *value_name,
                    const char *description,
                    double &dest);

    void new_named_double(const char *key, const char *long_name,const char *value_name,
                          const char *description,
                          double &dest);
    void new_optional_double(const char *value_name,
                             const char *description,
                             double &dest);

    void new_char(const char *value_name,
                  const char *description,
                  char &dest);
    void new_named_char(const char *key, const char *long_name,const char *value_name,
                        const char *description,
                        char &dest);
    void new_optional_char(const char *value_name,
                           const char *description,
                           char &dest);

    void new_unsigned_int(const char *value_name, const char *description,
                          unsigned int &dest);
    void new_optional_unsigned_int(const char *value_name, const char *description,
                                   unsigned int &dest);
    void new_named_unsigned_int(const char *key, const char *long_name,
                                const char *value_name, const char *description,
                                unsigned int &dest);

    void new_named_string_vector(const char *key, const char *long_name,
                                 const char *value_name, const char *description,
                                 std::vector<std::string> &dest);

    void set_string_vector(const char *arg_description, const char *description, std::vector<std::string> &dest);

    void set_author(const char *author);

    void set_description(const char *descr);

    void set_version(float v);
    void set_version(const char *str);

    void set_name(const char *name);

    void set_build_date(const char *date);

    void process(int argc, const char **argv);
    void process(int argc, char **argv) {
        process(argc, const_cast<const char **>(argv));
    }

    void write_usage(std::ostream &out) const;
    void write_values(std::ostream &out) const;

    ~ArgumentParser();
protected:
    typedef std::map<std::string, Argument_target*> SMap;
    typedef std::map<std::string, Argument_target*> LMap;
    typedef std::vector<Argument_target*> UVect;
    SMap short_names_;
    LMap long_names_;
    std::string author_;
    std::string name_;
    std::string description_;
    std::string date_;
    float version_;
    bool seen_end_named_;

    std::vector<Argument_target*> unnamed_arguments_;
    std::vector<Argument_target*> optional_unnamed_arguments_;
    std::vector<Argument_target*> all_arguments_;
    std::string extra_arguments_descr_;
    std::string extra_arguments_arg_descr_;
    std::vector<std::string> *extra_arguments_;
    std::vector<Argument_target*>::iterator current_unnamed_;
    std::vector<Argument_target*>::iterator current_optional_unnamed_;
    void new_argument_target(Argument_target*);
    void handle_error() const;
private:
    ArgumentParser(const ArgumentParser &) {};
    const ArgumentParser& operator=(const ArgumentParser &) {
        return *this;
    }
};


bool verbose=false, VERBOSE=false;

class ArgumentParser::Argument_target {
public:
    std::string key;
    std::string long_name;
    std::string description;
    std::string arg_description;

    Argument_target(const std::string& k, const std::string& lname,
                    const std::string& descr,
                    const std::string& arg_descr) {
        key=k;
        long_name=lname;
        description=descr;
        arg_description=arg_descr;
    }
    Argument_target(const std::string& descr,
                    const std::string& arg_descr) {
        key="";
        long_name="";
        description=descr;
        arg_description=arg_descr;
    }
    virtual bool process(int &, const char **&)=0;
    virtual void write_name(std::ostream &out) const;
    virtual void write_value(std::ostream &out) const=0;
    virtual void write_usage(std::ostream &out) const;
    virtual ~Argument_target() {}
};

void ArgumentParser::Argument_target::write_name(std::ostream &out) const {
    if (key != "") out << '-' << key;
    else if (!long_name.empty()) out << "--" << long_name;
    else out << arg_description;
}


void ArgumentParser::Argument_target::write_usage(std::ostream &out) const {
    if (key != "") {
        out << '-' << key;
        out << "/--" << long_name;
    }
    out << ' ' << arg_description;
    out << "\t" << description;
    out << " Value: ";
    write_value(out);
    out << std::endl;
}

class ArgumentParser::FlagTarget: public ArgumentParser::Argument_target {
  public:
    bool &val;
    FlagTarget(const char *k, const char *lname,
               const char *descr,
               bool &b): Argument_target(std::string(k), std::string(lname), std::string(descr),
                                             std::string()),  val(b) {}
    virtual bool process(int &, const char **&) {
        val= !val;
        return true;
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }

    virtual void write_usage(std::ostream &out) const {
        if (key != "") {
            out << '-' << key;
            out << "/--" << long_name;
        }
        out << "\t" << description;
        out << " Value: ";
        write_value(out);
        out << std::endl;
    }
    virtual ~FlagTarget() {}
};

class ArgumentParser::DoubleTarget: public Argument_target {
  public:
    double &val;
    DoubleTarget(const char *k, const char *lname,
                 const char *arg_descr,
                 const char *descr, double &b): Argument_target(std::string(k), std::string(lname),
                             std::string(descr),
                             std::string(arg_descr)),  val(b) {}
    DoubleTarget(const char *arg_descr,
                 const char *descr, double &b): Argument_target(std::string(descr),
                             std::string(arg_descr)),  val(b) {}
    virtual bool process(int &argc, const char **&argv) {
        if (argc==0) {
            std::cerr << "Missing value for argument." << std::endl;
            return false;
        }
        if (sscanf(argv[0], "%le", &val) ==1) {
            --argc;
            ++argv;
            return true;
        }  else {
            std::cerr << "Double not found at " << argv[0] << std::endl;
            return false;
        }
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }
    virtual ~DoubleTarget() {}
};

class ArgumentParser::IntTarget: public Argument_target {
public:
    int &val;
    IntTarget(const char *arg_descr,
              const char *descr, int &b): Argument_target(std::string(descr), std::string(arg_descr)),
        val(b) {}
    IntTarget(const char *k, const char *lname,
              const char *arg_descr,
              const char *descr, int &b): Argument_target(std::string(k), std::string(lname),
                          std::string(descr),
                          std::string(arg_descr)),
        val(b) {}
    virtual bool process(int &argc, const char **&argv) {
        if (argc==0) {
            std::cerr << "Missing value for argument." << std::endl;
            return false;
        }
        if (sscanf(argv[0], "%d", &val) ==1) {
            --argc;
            ++argv;
            return true;
        }  else {
            std::cerr << "Integer not found at " << argv[0] << std::endl;
            return false;
        }
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }
    virtual ~IntTarget() {}
};

class ArgumentParser::UIntTarget: public Argument_target {
public:
    unsigned int &val;
    UIntTarget(const char *arg_descr,
               const char *descr, unsigned int &b): Argument_target(std::string(descr), std::string(arg_descr)),
        val(b) {}
    UIntTarget(const char *k, const char *lname,
               const char *arg_descr,
               const char *descr, unsigned int &b): Argument_target(std::string(k), std::string(lname),
                           std::string(descr),
                           std::string(arg_descr)),
        val(b) {}
    virtual bool process(int &argc, const char **&argv) {
        if (argc==0) {
            std::cerr << "Missing value for argument." << std::endl;
            return false;
        }
        if (sscanf(argv[0], "%ud", &val) ==1) {
            --argc;
            ++argv;
            return true;
        } else {
            std::cerr << "Unsigned integer not found at " << argv[0] << std::endl;
            return false;
        }
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }
    virtual ~UIntTarget() {}
};


class ArgumentParser::CharTarget: public Argument_target {
public:
    char &val;
    CharTarget(const char *k, const char *lname,
               const char *arg_descr,
               const char *descr, char &b): Argument_target(std::string(k), std::string(lname),
                           std::string(descr),
                           std::string(arg_descr)),  val(b) {}
    CharTarget(const char *arg_descr,
               const char *descr, char &b): Argument_target(std::string(descr),
                           std::string(arg_descr)),  val(b) {}
    virtual bool process(int &argc, const char **&argv) {
        if (argc==0) {
            std::cerr << "Missing value for argument." << std::endl;
            return false;
        }
        if (sscanf(argv[0], "%c", &val) ==1) {
            --argc;
            ++argv;
            return true;
        }  else {
            std::cerr << "Character not found at " << argv[0] << std::endl;
            return false;
        }
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }
    virtual ~CharTarget() {}
};


class ArgumentParser::StringTarget: public Argument_target {
public:
    std::string &val;
    StringTarget(const char *arg_descr,
                 const char *descr, std::string &b): Argument_target(std::string(descr),  std::string(arg_descr)),
        val(b) {}

    StringTarget(const char *k, const char *lname, const char *arg_descr,
                 const char *descr, std::string &b): Argument_target(std::string(k), std::string(lname),
                             std::string(descr), std::string(arg_descr)),
        val(b) {}

    virtual bool process(int &argc, const char **&argv) {
        if (argc==0) {
            std::cerr << "Missing string argument." << std::endl;
            return false;
        }
        val= argv[0];
        --argc;
        ++argv;
        return true;
    }
    virtual void write_value(std::ostream &out) const {
        out << val;
    }
    virtual ~StringTarget() {}
};


class ArgumentParser::StringVectorTarget: public Argument_target {
public:
    std::vector<std::string> &val;

    StringVectorTarget(const char *k, const char *lname, const char *arg_descr,
                       const char *descr, std::vector<std::string> &b): Argument_target(std::string(k), std::string(lname),
                                   std::string(descr), std::string(arg_descr)),
        val(b) {}

    virtual bool process(int &argc, const char **&argv) {
        while (argc >0 && argv[0][0] != '-') {
            val.push_back(argv[0]);
            --argc;
            ++argv;
        }
        return true;
    }
    virtual void write_value(std::ostream &out) const {
        for (unsigned int i=0; i< val.size(); ++i) {
            out << val[i] << " ";
        }
    }
    virtual ~StringVectorTarget() {}
};


ArgumentParser::ArgumentParser() {
    author_="a programmer";
    description_= "This program produces output.";
    date_= "some day a long, long time ago.";
    version_=-1;
    extra_arguments_=NULL;
    seen_end_named_=false;
    new_flag("v", "verbose", "Whether to print extra information", verbose);
    new_flag("V", "VERBOSE", "Whether to print lots of extra information", VERBOSE);
}



void ArgumentParser::set_string_vector(const char *arg_description,
                                        const char *description,
                                        std::vector<std::string> &dest) {
    assert(extra_arguments_==NULL);
    extra_arguments_descr_= description;
    extra_arguments_arg_descr_= arg_description;
    extra_arguments_= &dest;
}

void ArgumentParser::set_author(const char *author) {
    author_=author;
}

void ArgumentParser::set_description(const char *descr) {
    description_= descr;
}

void ArgumentParser::set_name(const char *descr) {
    name_= descr;
}

void ArgumentParser::set_version(float v) {
    version_=v;
}

void ArgumentParser::set_version(const char *s) {
    version_=atof(s);
}

void  ArgumentParser::set_build_date(const char *date) {
    date_=date;
}

void ArgumentParser::new_argument_target(Argument_target *t) {
    assert(t!= NULL);
    if (t->key != "") {
        if (short_names_.find(t->key) != short_names_.end()) {
            std::cerr << "Two arguments are defined with the same string key, namely" << std::endl;
            short_names_[t->key]->write_usage(std::cerr);
            std::cerr << "\n and \n";
            t->write_usage(std::cerr);
            std::cerr << std::endl;
        }
        short_names_[t->key]= t;
    }
    if (!t->long_name.empty()) {
        if (long_names_.find(t->long_name) != long_names_.end()) {
            std::cerr << "Two arguments are defined with the same long key, namely" << std::endl;
            long_names_[t->long_name]->write_usage(std::cerr);
            std::cerr << "\n and \n";
            t->write_usage(std::cerr);
            std::cerr << std::endl;
        }
        long_names_[t->long_name]= t;
    }
    all_arguments_.push_back(t);
}

void ArgumentParser::new_flag(const char *key, const char *long_name, const char *description,bool &dest) {
    Argument_target *t= new FlagTarget(key, long_name, description, dest);
    new_argument_target(t);
};



void ArgumentParser::new_string(const char *arg_description, const char *description,
                                 std::string &dest) {
    Argument_target *t= new StringTarget(arg_description, description, dest);
    unnamed_arguments_.push_back(t);
    all_arguments_.push_back(t);
};
void ArgumentParser::new_optional_string(const char *arg_description, const char *description,
        std::string &dest) {
    Argument_target *t= new StringTarget(arg_description, description, dest);
    optional_unnamed_arguments_.push_back(t);
};
void ArgumentParser::new_named_string(const char *key, const char *long_name,
                                       const char *arg_description, const char *description,
                                       std::string &dest) {
    Argument_target *t= new StringTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};


void ArgumentParser::new_named_string_vector(const char *key, const char *long_name,
        const char *arg_description, const char *description,
        std::vector<std::string> &dest) {
    Argument_target *t= new StringVectorTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};



void ArgumentParser::new_int(const char *arg_description, const char *description,
                              int &dest) {
    Argument_target *t= new IntTarget(arg_description, description, dest);
    unnamed_arguments_.push_back(t);
    all_arguments_.push_back(t);
};
void ArgumentParser::new_optional_int(const char *arg_description, const char *description,
                                       int &dest) {
    Argument_target *t= new IntTarget(arg_description, description, dest);
    optional_unnamed_arguments_.push_back(t);
};
void ArgumentParser::new_named_int(const char *key, const char *long_name,
                                    const char *arg_description, const char *description,
                                    int &dest) {
    Argument_target *t= new IntTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};

void ArgumentParser::new_unsigned_int(const char *arg_description, const char *description,
                                       unsigned int &dest) {
    Argument_target *t= new UIntTarget(arg_description, description, dest);
    unnamed_arguments_.push_back(t);
    all_arguments_.push_back(t);
};
void ArgumentParser::new_optional_unsigned_int(const char *arg_description, const char *description,
        unsigned int &dest) {
    Argument_target *t= new UIntTarget(arg_description, description, dest);
    optional_unnamed_arguments_.push_back(t);
};
void ArgumentParser::new_named_unsigned_int(const char *key, const char *long_name,
        const char *arg_description, const char *description,
        unsigned int &dest) {
    Argument_target *t= new UIntTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};


void ArgumentParser::new_double(const char *arg_description, const char *description,
                                 double &dest) {
    Argument_target *t= new DoubleTarget(arg_description, description, dest);
    unnamed_arguments_.push_back(t);
    all_arguments_.push_back(t);
};
void ArgumentParser::new_optional_double(const char *arg_description, const char *description,
        double &dest) {
    Argument_target *t= new DoubleTarget(arg_description, description, dest);
    optional_unnamed_arguments_.push_back(t);
};
void ArgumentParser::new_named_double(const char *key, const char *long_name,
                                       const char *arg_description, const char *description,
                                       double &dest) {
    Argument_target *t= new DoubleTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};

void ArgumentParser::new_char(const char *arg_description, const char *description,
                               char &dest) {
    Argument_target *t= new CharTarget(arg_description, description, dest);
    unnamed_arguments_.push_back(t);
    all_arguments_.push_back(t);
};
void ArgumentParser::new_optional_char(const char *arg_description, const char *description,
                                        char &dest) {
    Argument_target *t= new CharTarget(arg_description, description, dest);
    optional_unnamed_arguments_.push_back(t);
};
void ArgumentParser::new_named_char(const char *key, const char *long_name,
                                     const char *arg_description, const char *description,
                                     char &dest) {
    Argument_target *t= new CharTarget(key, long_name, arg_description, description, dest);
    new_argument_target(t);
};



void ArgumentParser::write_usage(std::ostream &out) const {
    out << name_ << " version " << version_ << ", by " << author_ << std::endl;
    out << description_ << std::endl;
    out << "Compiled on " << date_ << std::endl << std::endl;
    out << "Usage: " << name_  << " ";
    for (UVect::const_iterator it= unnamed_arguments_.begin(); it != unnamed_arguments_.end(); ++it) {
        (*it)->write_name(out);
        out << " ";
    }
    for (UVect::const_iterator it= optional_unnamed_arguments_.begin();
            it != optional_unnamed_arguments_.end(); ++it) {
        out << "[";
        (*it)->write_name(out);
        out << "] ";
    }
    if (extra_arguments_ != NULL) {
        out << "[" << extra_arguments_arg_descr_ << "]";
    }

    out << std::endl << std::endl;
    out << "All arguments:\n";
    for (UVect::const_iterator it= unnamed_arguments_.begin(); it != unnamed_arguments_.end(); ++it) {
        (*it)->write_usage(out);
    }
    for (UVect::const_iterator it= optional_unnamed_arguments_.begin();
            it != optional_unnamed_arguments_.end(); ++it) {
        (*it)->write_usage(out);
    }

    if (!extra_arguments_arg_descr_.empty()) {
        out << extra_arguments_arg_descr_ << ": " << extra_arguments_descr_ << std::endl;
    }
    for (SMap::const_iterator it= short_names_.begin(); it != short_names_.end(); ++it) {
        (it->second)->write_usage(out);
    }
}



void ArgumentParser::write_values(std::ostream &out) const {
    for (UVect::const_iterator it= unnamed_arguments_.begin(); it != unnamed_arguments_.end(); ++it) {
        out << (*it)->description;
        out << ": ";
        (*it)->write_value(out);
        out << std::endl;
    }
    for (UVect::const_iterator it= optional_unnamed_arguments_.begin();
            it != optional_unnamed_arguments_.end(); ++it) {
        out << (*it)->description;
        out << ": ";
        (*it)->write_value(out);
        out << std::endl;
    }
    if (extra_arguments_!=NULL) {
        for (std::vector<std::string>::const_iterator it= extra_arguments_->begin();
                it != extra_arguments_->end(); ++it) {
            out << *it << " ";
        }
    }

    for (SMap::const_iterator it= short_names_.begin(); it != short_names_.end(); ++it) {
        out << it->second->description;
        out << ": ";
        it->second->write_value(out);
        out << std::endl;
    }
}

ArgumentParser::~ArgumentParser() {
    for (std::vector<Argument_target*>::iterator it= all_arguments_.begin();
            it != all_arguments_.end(); ++it) {
        delete *it;
    }
}


void ArgumentParser::process(int argc,  const char **argv) {
    name_= argv[0];
    ++argv;
    --argc;

    current_unnamed_= unnamed_arguments_.begin();
    current_optional_unnamed_= optional_unnamed_arguments_.begin();

    for ( int i=0; i< argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            write_usage(std::cout);
            exit(0);
        }
    }

    while (argc != 0) {

        const char* cur_arg= argv[0];
        if (cur_arg[0]=='-' && !seen_end_named_) {
            --argc;
            ++argv;
            if (cur_arg[1]=='-') {
                if (cur_arg[2] == '\0') {
                    seen_end_named_=true;
                } else {
                    LMap::iterator f= long_names_.find(cur_arg+2);
                    if ( f != long_names_.end()) {
                        if (!f->second->process(argc, argv)) {
                            handle_error();
                        }
                    } else {
                        std::cerr<< "Invalid long argument "<< cur_arg << ".\n";
                        handle_error();
                    }
                }
            } else {
                if (cur_arg[1]=='\0') {
                    std::cerr << "Invalid argument " << cur_arg << ".\n";
                    handle_error();
                }
                SMap::iterator f= short_names_.find(cur_arg+1);
                if ( f != short_names_.end()) {
                    if (!f->second->process(argc, argv)) {
                        handle_error();
                    }
                } else {
                    std::cerr<< "Invalid short argument "<< cur_arg << ".\n";
                    handle_error();
                }
            }
        } else {
            if (current_unnamed_ != unnamed_arguments_.end()) {
                Argument_target *t= *current_unnamed_;
                t->process(argc, argv);
                ++current_unnamed_;
            } else if (current_optional_unnamed_ != optional_unnamed_arguments_.end()) {
                Argument_target *t= *current_optional_unnamed_;
                t->process(argc, argv);
                ++current_optional_unnamed_;
            } else if (extra_arguments_!= NULL) {
                extra_arguments_->push_back(cur_arg);
                --argc;
                ++argv;
            } else {
                std::cerr << "Invalid extra argument " << argv[0] << std::endl;
                handle_error();
            }
        }
    }

    if (current_unnamed_ != unnamed_arguments_.end()) {
        std::cerr << "Missing required arguments:" << std::endl;
        for (; current_unnamed_ != unnamed_arguments_.end(); ++current_unnamed_) {
            (*current_unnamed_)->write_name(std::cerr);
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
        handle_error();
    }

    if (VERBOSE) verbose=true;
}

void ArgumentParser::handle_error() const {
    write_usage(std::cerr);
    exit(1);
}

}

#endif // SRC_LYX_UTLIS_ArgumentParser_H_